cmake_minimum_required(VERSION 3.16)
project(VideoPlayer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_link_options(-static-libstdc++ -static-libgcc)
add_executable(VideoPlayer
    main.cpp
    VideoPlayer.cpp
)

set_target_properties(VideoPlayer PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(VideoPlayer PRIVATE
    Qt6::Widgets
)

if(MSVC)
  target_compile_options(VideoPlayer PRIVATE /W4 /WX)
else()
  target_compile_options(VideoPlayer PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()