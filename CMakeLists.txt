# config:
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_PREFIX_PATH
    "D:/cpp/lib/Qt/6.9.1/msvc2022_64/lib/cmake/Qt6"
    "D:/cpp/lib/opencv/build"
)
# set(CMAKE_INCLUDE_PATH "D:/c++/tools/msys64/ucrt64/include/opencv4")
# set(CMAKE_BUILD_TYPE Debug)
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
    frame_indexer.cpp frame_indexer.h
    vp.rc # for icon
)
qt_add_resources(${PROJECT_NAME} "icons"
    PREFIX "/"
    FILES
        icon/play.svg
        icon/pause.svg
        icon/enter-fullscreen.svg
        icon/exit-fullscreen.svg
        icon/loop-active.svg
        icon/loop-inactive.svg
        icon/always-on-top-active.svg
        icon/always-on-top-inactive.svg
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
    opencv_core
    opencv_videoio
)

if(MSVC)
  # target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

qt_add_ios_ffmpeg_libraries(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
