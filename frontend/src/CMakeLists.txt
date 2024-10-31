cmake_minimum_required(VERSION 3.16)

project(leerhulpmiddel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set(SOURCES
    main.cpp
    model/Questions/Question.cpp
)

set(HEADERS
    model/Questions/Question.h
)

qt_add_executable(leerhulpmiddel ${SOURCES} ${HEADERS})

target_link_libraries(leerhulpmiddel PRIVATE Qt6::Core Qt6::Widgets)

set_target_properties(leerhulpmiddel PROPERTIES
    WIN32_EXECUTABLE ON
)