cmake_minimum_required(VERSION 3.5)

project(view VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

#set(CMAKE_CXX_FLAGS "-lGL -lGLU -lglut")

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        s21_viewer_3d.c
        s21_viewer_3d.h
        viewer.h
        viewer.cpp
        QtGifImage-master/src/gifimage/qgifglobal.h
        QtGifImage-master/src/gifimage/qgifimage.h
        QtGifImage-master/src/gifimage/qgifimage_p.h
        QtGifImage-master/src/gifimage/qgifimage.cpp
        QtGifImage-master/src/3rdParty/giflib/dgif_lib.c
        QtGifImage-master/src/3rdParty/giflib/egif_lib.c
        QtGifImage-master/src/3rdParty/giflib/gif_err.c
        QtGifImage-master/src/3rdParty/giflib/gif_hash.c
        QtGifImage-master/src/3rdParty/giflib/gifalloc.c
        QtGifImage-master/src/3rdParty/giflib/quantize.c
        QtGifImage-master/src/3rdParty/giflib/gif_lib.h
        QtGifImage-master/src/3rdParty/giflib/gif_lib_private.h
        QtGifImage-master/src/3rdParty/giflib/gif_hash.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(view
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET view APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(view SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(view
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(view PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

set_target_properties(view PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(view)
endif()
