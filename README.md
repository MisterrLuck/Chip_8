# Chip 8

A Chip 8 emulator written in C++ with the Raylib graphics library.

Tested using the [Test ROM](https://github.com/corax89/chip8-test-rom)

## Configuration

There are many different versions of the chip 8 emulator. Some have differing instruction behaviour. To be able to run as many roms as possible, you can change the configuration in the CMakeLists.txt

```CMakeLists.txt
# Configure the version of chip8
add_compile_definitions(SHIFT)
add_compile_definitions(JUMP_OFFSET)
```

[Here](CONFIG.md) is the full list of what each change does.


## Usage

Building the Chip 8 emulator

```bash
mkdir build
cd build
cmake ..
make
./Chip_8
```

You can use the `code_to_rom.py` file to convert your code from a text file to a binary format to be run

```bash
python code_to_rom.py
```

If you want to make it easier to recompile you programs quickly, use the command line arguments instead

```bash
python code_to_rom.py input_file.txt output_file.ch8
```

This python program will also allow for comments with `#`
