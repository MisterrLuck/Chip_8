# Chip 8

A Chip 8 emulator written in C++ with the Raylib graphics library.

## Table of Contents

- [Specs](#specs)
- [Configuration](#configuration)
- [Requirements](#requirements)
- [Usage](#usage)
- [Programs](#programs)
    - [Creating Programs](#creating-programs)
    - [Debugging Programs](#debugging-programs)
- [Thanks](#thanks)
- [What's Next](#whats-next)


## Specs

Memory -> 4096 Bytes

opcode -> 2 Bytes


## Configuration

There are many different versions of the chip 8 emulator. Some have differing instruction behaviour. To be able to run as many roms as possible, you can change the configuration in the CMakeLists.txt

```cmake
add_compile_definitions(SUPER_CHIP) # Changes to the SUPER-CHIP version of Chip 8
```

[Here](docs/CONFIG.md) is the full list of what each change does.


## Requirements

- cmake
- g++
- [raylib](https://github.com/raysan5/raylib#build-and-installation)


## Usage

Building the Chip 8 emulator

```bash
mkdir build
cd build
cmake ..
make
./Chip_8
```


## Programs

### Creating Programs

You can use the `code_to_rom.py` file to convert your code from a text file to a binary format to be run

```bash
python code_to_rom.py
```

If you want to make it easier to recompile you programs quickly, use the command line arguments instead

```bash
python code_to_rom.py input_file.txt output_file.ch8
```

Or even better, if you want them to have the same name just do the input file
```bash
python code_to_rom.py myRom.txt
```

This python program will also allow for comments with `#`


### Debugging Programs

You can also debug your own programs using the custom debugger.
More information can be found [here](docs/DEBUGGER.md)

It is in very early stages.


## Thanks

Thanks to Tobias V. I. Langhoff and his [blog post](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) on making a Chip 8 emulator
Thanks to cj1128 on github for his [BC_test rom](https://github.com/cj1128/chip8-emulator/blob/master/rom/BC_test.ch8) which helped debugging.


## Whats Next

Some things I want to add in the future

 - Improve the debugger to maybe add breakpoints and have a separate window running at once, inspired by [massung](https://github.com/massung/CHIP-8) or [Austin Morlan](https://code.austinmorlan.com/austin/2019-chip8-emulator). This would mean moving away from raylib which would suck, because raylib doesn't support multiple windows.
 - Create an assembler which was also inspired by [massung](https://github.com/massung/CHIP-8).
 - Improve the overall experience, with easier changing between instruction sets, without rebuilding the whole program
 - Add proper [SUPER-CHIP](https://johnearnest.github.io/Octo/docs/SuperChip.html) support with the larger screen and extra instructions, instead of just the inconsistencies in existing instructions
 - Maybe even adding [XO-CHIP](https://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html) functionality
