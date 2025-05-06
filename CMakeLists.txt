cmake_minimum_required(VERSION 3.15)

project(viewer VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

set(target_app ${PROJECT_NAME})

add_executable(${target_app})

find_package(Qt5 REQUIRED COMPONENTS 
    Core
    Gui
    Widgets 
    Xml
    Sql
)

file(GLOB_RECURSE SOURCES
    src/*.h
    src/*.cpp
)

target_sources(${target_app} PRIVATE ${SOURCES})

set_target_properties(${target_app} PROPERTIES AUTOMOC ON)

target_compile_definitions(${target_app} PRIVATE TEST_FILES_FOLDER="${PROJECT_SOURCE_DIR}/data/tests/TestFolder")

target_link_libraries(${target_app} PRIVATE 
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    Qt5::Sql
)
