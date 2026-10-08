# Debugger

This custom debugger allows for viewing code easier, with a distinct separation between opcodes and sprite data.


## Usage

By default the CMakeLists file will compile both a debugger and non debugger version of the emulator. Run the debugger version just like you would the normal version.

```bash
./Debug_Chip_8 ../roms/myRom.ch8
```

It will then prompt you for commands at every instruction.


## List of commands

You can only enter one character for commands.


| Command | Description | 
|---------|-------------|
| p | print out the entire program |
| n | run the next opcode |
| l | print opcode that just ran |
| o | print next opcode |
| v | print registers |
| i | print index |
| d | print delay timer |
| s | print sound timer |
| c | continue the program to the end |
| e | exit the program | 



## Specs

The debugger will disassemble the given code to make it easier to debug. It first branches through the program to see which bytes are opcodes, and which is sprite data.

> This may be wrong because of the BNNN instruction, which allows for different jump locations depending on the current state.

It then has a list of locations of the program, and for the data. When printing out the program, it uses these locations for different formatting.
