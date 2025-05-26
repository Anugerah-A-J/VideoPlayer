VideoPlayer

[-] Show time over duration (layout problem)
[ ] Show map when zoom scale > 100 %
[ ] Loop button
[ ] Stay on top button

[x] Double click on mouse left button to enter/exit fullscreen
[x] Hold mouse left button to play 2x
[-] Single click on mouse left button to play/pause (conflict with mouse double click)
[ ] Hover over slider to show thumbnail
[ ] Hold mouse right button and scroll mouse wheel to zoom in
[ ] Drag using mouse right button to pan when zoom scale > 100 %

[-] Hit right key to skip forward 7 sec. (not showing control panel)
[-] Hit left key to skip backward 7 sec. (not showing control panel)
[ ] Hold space key to play 2x
[ ] Hit space key to play/pause

[ ] Simple editing like merge and cut

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build