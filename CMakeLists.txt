cmake_minimum_required(VERSION 3.10)

project(AsciiDonut VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)

add_executable(AsciiDonut main.cpp scene.cpp)

set_target_properties(AsciiDonut PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(AsciiDonut PRIVATE ${CURSES_INCLUDE_DIRS})

target_link_libraries(AsciiDonut PRIVATE ${CURSES_LIBRARIES})