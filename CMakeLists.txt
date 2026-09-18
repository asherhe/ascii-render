cmake_minimum_required(VERSION 3.10)

project(AsciiDonut VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)

add_executable(AsciiDonut
    src/main.cpp
    src/scene.cpp
    src/render.cpp
)

set_target_properties(AsciiDonut PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(AsciiDonut PRIVATE ${CURSES_INCLUDE_DIRS})

target_link_libraries(AsciiDonut PRIVATE ${CURSES_LIBRARIES})

# python venv configuration - requires uv

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(VENV_DIR "${CMAKE_BINARY_DIR}/.venv")
set(VENV_PYTHON "${VENV_DIR}/bin/python")

# create venv if it doesn't exist
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
