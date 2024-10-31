# Description

## Prerequisites
[SDL2](https://wiki.libsdl.org/SDL2/FrontPage) must be installed.

## Usage

These are desktop "emulators" of the [Map](../Map/Map.ino) and 
[Ardymo](../Ardymo/Ardymo.ino) applications. They are useful for debugging 
and testing the algorithms. There is no emulation of I2C communication. The 
I2C calls are ignored. The programs run as if I2C were disabled. But they 
compile fine with `#define USE_I2C` set.

`make ardymo` compiles the Ardymo emulator and
`make map` compiles the Map emulator.

`make` compiles both.
`make clean` removes the executables and object files.

Run the applications with `./map` and `./ardymo` respectively. A command line argument of 1, 2, ... determines the pixels on the computer for one pixel on the Arduboy screen. For a size equivalent to the Arduboy screen, use 1 (one pixel on the computer screen is one pixel on the Arduboy screen). The default is 4.

