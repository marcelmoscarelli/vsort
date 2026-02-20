# vsort

## Description

A sorting visualizer using ImGui and SDL2 as rendering backend.

There is not much to see here, yet.

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

## Controls

Apart from the GUI, there are a few keyboard shortcuts that let you control the simulation without touching the mouse:

- Backspace: Shuffle

- Space bar: Start/Stop

- Up arrow and down arrow: Cycle through algorithms

- Esc: Quit