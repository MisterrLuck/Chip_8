# Chip 8

A Chip 8 emulator written in C++ with the Raylib graphics library.

Aside from the emulator, it currently includes a text to rom converter (to write programs in hexadecimal easily), a custom debugger, and an assembler.

This is my first emulation project to get me into the genre. It has been super fun to make, and there is a great community surrounding it.

## Table of Contents

- [Features](#features)
    - [Emulator](#emulator)
        - [Configuration](#configuration)
    - [Debugger](#debugger)
    - [Assembler](#assembler)
    - [Hex Code](#hex-code)
- [Building](#building)
    - [Requirements](#requirements)
    - [Compilation](#compiling-from-source)
- [Thanks](#thanks)
- [What's Next](#whats-next)


## Features

### Emulator

The Emulator is run fully from the command line. To run a program, just provide it as an argument.

```
./Chip_8 myRom.ch8
```

You can also change the Chip 8 version with the configuration flag.

```
./Chip_8 -c chip-48 myRom.ch8
```

#### Configuration

A list of all the current configuration options can be found [here](docs/configuration.md).

### Debugger

You can also debug programs using the custom debugger. Just add the debug flag when running.

``` bash
./Chip_8 -d myRom.ch8
```

This will make the program pause after every instruction, and wait for a command input.
A list of all the instructions for the debugger can be found [here](docs/debugger.md).

### Assembler

An assembler is also provided separately from the emulator.

It is a custom made assembly language and **not** an octo assembler. The reference can be found [here](docs/assembly.md).

When it's run with only an input file, the output will default to `out.ch8`.

```
./Assembler myRom.asm
```

You can specify an output file using the `-o` flag.

```
./Assembler myRom.asm -o myRom.ch8
```

Changing the configuration works the same way as it does for the emulator.

```
./Assembler -c chip-8 myRom.asm -o myRom.ch8
```


### Hex Code

You can use the `scripts/code_to_rom.py` file to convert your code from text to a binary file, that you can run.

```bash
python code_to_rom.py
```

If you want to make it easier to recompile you programs quickly, use the command line arguments instead.

```bash
python code_to_rom.py input_file.txt output_file.ch8
```

Or even better, if you want them to have the same name, just provide the input file.

```bash
python code_to_rom.py myRom.txt
```

This will write to myRom.ch8.

The converter also allows you to document your code with comments, using `#`.


## Building

Compilation has currently only been tested for Linux. Specifically on Ubuntu with an Intel processor.

> If you have tested building on another device, please let me know.

### Requirements

Make sure you have all the requirements before building.

- C++17
- CMake (Minimum Version 4.2)
- [raylib](https://github.com/raysan5/raylib#build-and-installation)
- [CLI11](https://github.com/cliutils/cli11)

Raylib is installed to the system and found through CMake. CLI11 should be just the header file, installed to the `include/utils` directory.

### Compiling from Source

After installing all the requirements, you can build the Emulator and Assembler using CMake.

```bash
mkdir build
cd build
cmake ..
make
```

Run the emulator with your desired ROM as an input. A test program, `roms/default_program.ch8`, is provided to ensure everything was set up correctly. 


## Thanks

Thanks to Tobias V. I. Langhoff and his [blog post](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) on making a Chip 8 emulator.

Thanks to Timendus for his [Chip-8 Test Suite](https://github.com/Timendus/chip8-test-suite) which fixed most issues.

Thanks to Gulrak for help debugging and also for inspiration on his emulator [Cadmium](https://github.com/gulrak/cadmium).

Thanks to Various people in the EmuDev discord server for guidance.


## Whats Next

Some things I want to add in the future

 - Improve the debugger to have a separate window running at once, inspired by [massung](https://github.com/massung/CHIP-8), [Austin Morlan](https://code.austinmorlan.com/austin/2019-chip8-emulator), or [Gulrak's Cadmium](https://github.com/gulrak/cadmium).
 - Possibly move away from Raylib to learn a more common graphics library like SDL.
 - Add proper [SUPER-CHIP](https://johnearnest.github.io/Octo/docs/SuperChip.html) support with the larger screen and extra instructions, instead of just the inconsistencies in existing instructions.
 - Maybe even adding [XO-CHIP](https://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html) functionality.
 - Eventually work in as many CHIP 8 versions as possible.
