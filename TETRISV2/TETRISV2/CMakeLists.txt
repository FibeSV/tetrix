cmake_minimum_required(VERSION 3.16)
project(tetris LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/widgets/tetrix")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_add_executable(tetris
    host_window.h host_window.cpp
    main.cpp
    tetrisboard.cpp tetrisboard.h
    tetrispiece.cpp tetrispiece.h
    tetriswindow.cpp tetriswindow.h
)

set_target_properties(tetris PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(tetris PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

install(TARGETS tetris
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
