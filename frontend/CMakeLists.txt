cmake_minimum_required(VERSION 3.16)

project(leerhulpmiddel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets GUI)
qt_standard_project_setup()

set(CMAKE_AUTOMOC ON)  # Important for QObject-derived classes
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add all source files in the project
file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/**/*.h"
)

message(STATUS "Project sources: ${PROJECT_SOURCES}")

qt_add_executable(leerhulpmiddel
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_link_libraries(leerhulpmiddel PUBLIC  Qt6::Widgets Qt6::Gui)


set_target_properties(leerhulpmiddel PROPERTIES
    WIN32_EXECUTABLE ON
)