cmake_minimum_required(VERSION 3.16)

project(AsciiDonut VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(AsciiDonut
    src/main.cpp
    src/scene.cpp
    src/render.cpp
)

set_target_properties(AsciiDonut PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ==============================================================================
# Curses Platform Selection
# ==============================================================================
if(WIN32)
    # Native Windows Build (PDCursesMod Win32 Console)
    include(FetchContent)

    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_BINARY_DIR}/install"
        CACHE PATH "Local install prefix" FORCE
    )
    set(PDC_SDL2_BUILD OFF CACHE BOOL "Build SDL2 project" FORCE)
    set(PDC_SDL2_DEPS_BUILD OFF CACHE BOOL "Build SDL2 dependencies" FORCE)
    set(PDC_GL_BUILD OFF CACHE BOOL "Build OpenGL project" FORCE)
    set(PDC_WIDE ON CACHE BOOL "Build wide-character PDCurses" FORCE)

    FetchContent_Declare(
        pdcursesmod
        GIT_REPOSITORY https://github.com/Bill-Gray/PDCursesMod.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(pdcursesmod)

    target_link_libraries(AsciiDonut PRIVATE wincon_pdcurses)
    target_include_directories(AsciiDonut PRIVATE ${pdcursesmod_SOURCE_DIR})
    target_compile_definitions(AsciiDonut PRIVATE PDC_WIDE)
    # move libpdcurses.dll to same folder as AsciiDonut.exe so that it can be found
    add_custom_command(TARGET AsciiDonut POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:wincon_pdcurses>
            $<TARGET_FILE_DIR:AsciiDonut>
    )

else()
    # POSIX / Linux / macOS Build (Native System ncurses)
    find_package(Curses REQUIRED)
    target_include_directories(AsciiDonut PRIVATE ${CURSES_INCLUDE_DIRS})
    target_link_libraries(AsciiDonut PRIVATE ${CURSES_LIBRARIES})
endif()

# ==============================================================================
# Python venv configuration - requires uv
# ==============================================================================
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(VENV_DIR "${CMAKE_BINARY_DIR}/.venv")

# Adjust executable path according to OS platform
if(WIN32)
    set(VENV_PYTHON "${VENV_DIR}/Scripts/python.exe")
else()
    set(VENV_PYTHON "${VENV_DIR}/bin/python")
endif()

# Create venv if it doesn't exist
if(NOT EXISTS "${VENV_DIR}")
    execute_process(
        COMMAND uv venv "${VENV_DIR}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    execute_process(
        COMMAND uv pip install -r requirements.txt --python "${VENV_PYTHON}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
