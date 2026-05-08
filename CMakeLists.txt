cmake_minimum_required(VERSION 3.31)
project(simple_notepad_project)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg PrintSupport)

qt_standard_project_setup()

qt_add_executable(notepad
        main.cpp
        main_window.cpp
        main_window.h
        text_transform.h
        notepad_exception.h
        spell_checker.h
        spell_checker_highlighter.h
)

target_link_libraries(notepad PRIVATE
        Qt6::Widgets
        Qt6::Svg
        Qt6::PrintSupport
)