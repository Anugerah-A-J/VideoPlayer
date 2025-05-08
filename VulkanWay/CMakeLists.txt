# cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G Ninja -B build

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyVideoPlayer)

find_package(glfw3)
find_package(VulkanLoader)

add_link_options(-static-libstdc++ -static-libgcc)

add_executable(mvp
    main.cpp
)

if(MSVC)
    target_compile_options(mvp PRIVATE /W4 /WX)
else()
    target_compile_options(mvp PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(mvp PRIVATE
    glfw3
    vulkan-1
)