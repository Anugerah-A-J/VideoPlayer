VideoPlayer

[x] Show time over duration (layout problem)
[ ] Show map when zoom scale > 100 %
[ ] Loop button
[ ] Stay on top button

[ ] Double click on mouse left button to enter/exit fullscreen      Wait for testing
[ ] Hold mouse left button to play 2x                               Wait for testing
[ ] Single click on mouse left button to play/pause                 Wait for testing
[ ] Hover over slider to show thumbnail
[ ] Hold mouse right button and scroll mouse wheel to zoom in
[ ] Drag using mouse right button to pan when zoom scale > 100 %

[x] Hit right key to skip forward 7 sec.    Wait for testing
[x] Hit left key to skip backward 7 sec.    Wait for testing
[x] Hold space key to play 2x               Wait for testing
[x] Release space key to play/pause         Wait for testing

[ ] Simple editing like merge and cut

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G Ninja -B build