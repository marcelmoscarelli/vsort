# vsort

![vsort](https://github.com/user-attachments/assets/29eeeb7c-925b-4259-ba28-5c9409340ea3)

## Description

A sorting visualizer using ImGui and SDL2 as rendering backend.

Work in progress, more algorithms will be added.

First public beta was released on 01/mar/26 for Windows and Linux (x86_64). MacOS users can build using the intructions below.

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

## Building

### Getting Linux dependencies

Debian/Ubuntu/Mint:

```bash
sudo apt install g++ make pkg-config libsdl2-dev
```

Fedora/CentOS:

```bash
sudo dnf install gcc-c++ make pkgconf-pkg-config SDL2-devel
```

Arch:

```bash
sudo pacman -S base-devel sdl2
```

### Getting MacOS dependencies

Run:

```bash
xcode-select --install
```

```bash
brew install pkg-config sdl2
```

### Getting Windows dependencies

Download the following:

- [w64devkit](https://github.com/skeeto/w64devkit)
(then add its bin folder to the PATH variable, so you can access build tools from cmd or pwsh)

- [libsdl2-dev](https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-mingw.zip)
(extract it somewhere then add its \lib\pkgconfig folder to the PKG_CONFIG_PATH variable)

### Build and Run

```bash
make
```

```bash
make run
```

## How to use

Apart from the GUI controls, there are a few keyboard shortcuts that let you control the simulation without touching the mouse:

| Key | Action |
| :--- | :--- |
| <kbd>⌫ Backspace</kbd> | Shuffle the array |
| <kbd>R</kbd> | Toggle repeated elements |
| <kbd>␣ Space</kbd> | Start or stop sorting |
| <kbd>↑</kbd> <kbd>↓</kbd> | Cycle through algorithms |
| <kbd>←</kbd> <kbd>→</kbd> | Resize the array |
| <kbd>+</kbd> <kbd>-</kbd> | Adjust the FPS cap (non-numpad) |
| <kbd>0</kbd> | Set FPS to uncapped (non-numpad) |
| <kbd>&lt;</kbd> <kbd>&gt;</kbd> | Change bar spacing (0-4) |
| <kbd>⎋ Esc</kbd> | Quit |
