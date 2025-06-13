# cmake -G Ninja -B build
# config:
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_INCLUDE_PATH "D:/c++/tools/msys64/ucrt64/include/opencv4")
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

cmake_minimum_required(VERSION 3.16)
project(vp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/multimedia/videographicsitem")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Multimedia MultimediaWidgets Widgets)
find_package(OpenCV REQUIRED)

qt_add_executable(${PROJECT_NAME}
    main.cpp
    video_player.cpp video_player.h
    control_panel.cpp control_panel.h
    position_slider.cpp position_slider.h
    # frame_indexer.cpp frame_indexer.h
    thumbnail.h thumbnail.cpp
)

# this close the terminal when app is running
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     WIN32_EXECUTABLE TRUE
#     MACOSX_BUNDLE TRUE
# )

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Multimedia
    Qt::MultimediaWidgets
    Qt::Widgets
    ffms2
    opencv_core
    opencv_videoio
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -O)

qt_add_ios_ffmpeg_libraries(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
