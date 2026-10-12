# Debugger

This custom debugger allows for viewing code easier, with a distinct separation between opcodes and sprite data.


## Usage

By default, the CMakeLists file will compile both a debugger and non debugger version of the emulator. Run the debugger version just like you would the normal version.

```bash
./Debug_Chip_8 ../roms/myRom.ch8
```

It will then prompt you for commands at every instruction.


## List of commands

You can only enter one character for commands.


| Command | Description | Parameter |
|:-------:|-----------|-------------|
| d       | Delete a breakpoint                                   | Address to delete in hexadecimal |
| l       | Print number of instructions from the current address | How many lines to print |
| b       | Add a breakpoint                                      | Breakpoint address to add |
| m       | Address in memory to print                            | Print the value in memory at a given address |
| i       | Print out all current breakpoints                     | None |
| p       | print out the entire program                          | None |
| n       |  run the next opcode                                  | None |
| o       | Print opcode that just ran                            | None |
| v       | Print all registers (V + I)                           | None |
| a       | Print current address and current opcode about to run | None |
| t       | Print both timers                                     | None |
| c       | Continue the program to the end or until a breakpoint | None |
| e       | exit the program                                      | None |



## Specs?

The debugger will disassemble the given code to make it easier to debug. It first branches through the program to see which bytes are opcodes, and which is sprite data.

> This may be wrong because of the BNNN instruction, which allows for different jump locations depending on the current state.

It then has a list of locations of the program, and for the data. When printing out the program, it uses these locations for different formatting.

There is also a lookup table where it will convert each opcode to the corrosponding assembly. The assembly documentation can be found [here](assembly.md).
