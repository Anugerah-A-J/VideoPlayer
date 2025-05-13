# cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G Ninja -B build

# cmake_minimum_required(VERSION 3.16)
# project(VideoPlayer LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Qt6 REQUIRED COMPONENTS
# 	Widgets
# 	MultimediaWidgets
# )

# qt_standard_project_setup()

# add_link_options(-static-libstdc++ -static-libgcc)
# add_executable(VideoPlayer
#     main.cpp
#     videoplayer.cpp
# )

# set_target_properties(VideoPlayer PROPERTIES
#     WIN32_EXECUTABLE TRUE
#     MACOSX_BUNDLE TRUE
# )

# target_link_libraries(VideoPlayer PRIVATE
#     Qt6::Widgets
#     Qt6::MultimediaWidgets
# )

# if(MSVC)
# 	target_compile_options(VideoPlayer PRIVATE /W4 /WX)
# else()
# 	target_compile_options(VideoPlayer PRIVATE -Wall -Wextra -Wpedantic -Werror)
# endif()

cmake_minimum_required(VERSION 3.16)
project(mvp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR "install")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Multimedia MultimediaWidgets Widgets)

qt_add_executable(mvp
    main.cpp
    videoplayer.cpp videoplayer.h
)

set_target_properties(mvp PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(mvp PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Multimedia
    Qt::MultimediaWidgets
    Qt::Widgets
)

qt_add_ios_ffmpeg_libraries(mvp)

install(TARGETS mvp
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)
