# vsort

## Description

A sorting visualizer using ImGui and SDL2 as rendering backend.

Work in progress, more algorithms will be added.

### Supported Algorithms

O(n^2)

- Bubble Sort

- Selection Sort

- Insertion Sort

- Comb Sort (w/ 1.3 gap factor and, in practice, performs much better than O(n²))

O(n^(3/2))

- Shell Sort (w/ Ciura constants as gap factors, then 2.25)

O(n log n)

- Quick Sort (w/ Middle Pivot, 3-way partition, Insertion Sort base case)

- Heap Sort

- Merge Sort (w/ Insertion Sort base case)

## Dependencies

### Linux

Get the following with your package manager:

- g++

- make

- pkg-config

- libsdl2-dev

### Windows

Get the following:

- [w64devkit](https://github.com/skeeto/w64devkit)
(then add its bin folder to the PATH variable, so you can access build tools from cmd or pwsh)

- [libsdl2-dev](https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-mingw.zip)
(extract it somewhere then add its \lib\pkgconfig folder to the PKG_CONFIG_PATH variable)

## Run

- `make run`

## How to use

Apart from the GUI controls, there are a few keyboard shortcuts that let you control the simulation without touching the mouse:

- Backspace: Shuffle the array

- Space bar: Start or stop sorting

- Up arrow and down arrow: Cycle through algorithms

- Left arrow and right arrow: Resize the array

- Plus and minus (not from numpad): Adjust the FPS cap

- Zero (not from numpad): Set FPS to uncapped

- Esc: Quit
