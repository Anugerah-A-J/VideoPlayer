VideoPlayer

[x] Show time over duration
[x] Show map when zoom scale > 100 %
[ ] Loop button
[ ] Stay on top button

[x] Double click on mouse left button to enter/exit fullscreen
[x] Hold mouse left button to play 2x
[x] Single click on mouse left button to play/pause
[x] Hover over slider to show thumbnail
[x] Hide cursor
[-] Hold mouse right button and scroll mouse wheel to zoom in
[-] Drag using mouse right button to pan when zoom scale > 100 %

[x] Hit right key to skip forward 7 sec.
[x] Hit left key to skip backward 7 sec.
[x] Hold space key to play 2x
[x] Release space key to play/pause

[ ] Simple editing like merge and cut

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build

<!--
vcpkg new --application
vcpkg add port qt
vcpkg integrate install
cmake -B build --fresh "-DCMAKE_TOOLCHAIN_FILE=D:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_EXPORT_COMPILE_COMMANDS=1

QT_PLUGIN_PATH="D:\cpp\pro\PassGenGUI\build\vcpkg_installed\x64-windows\Qt6\plugins\platforms"

Microsoft Visual C++ Runtime Library

Debug Error!

Program: D:\cpp\pro\PassGenGUI\build\Qt6Cored.dll
Module: 6.8.2
File:
D:\cpp\pro\PassGenGUl\build\vcpkg_installed\vcpkg\blds\qtbase\src\here-src-6-c23658a936.clean\src\gui\kernel\qguiapplication.cpp
Line: 1327

This application failed to start because no Qt platform plugin
could be initialized. Reinstalling the application may fix this
problem.

(Press Retry to debug the application)
-->