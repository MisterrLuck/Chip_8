# Debugger

The custom debugger allows for command inputs after every instruction.

The debugger employs a custom algorithm to search through the program and classify each byte as either code or sprite data.
When printing out the program, there is a distinct separation between opcodes and sprite data.

> This doesn't work with the BNNN instruction, so issues may arise.


## Prompt

When the prompt appears, enter a character corresponding to the command you are trying to do.
Some commands involve a parameter, which you can enter right after the character.

All of these are correct ways to enter a parameter.

```
>>b 290
```
```
>>b340
```
```
>>d
31A
```

Each parameter is listed as either decimal or hexadecimal. You don't need to add any identifiers to the beginning like `0x`.


## List of commands


| Command | Description | Parameter |
|:-------:|-----------|-------------|
| p       | Print out the entire program                          | N/A |
| l       | Print number of instructions from the current address | How many lines to print (decimal) |
| a       | Print current opcode about to run                     | N/A |
| o       | Print opcode that just ran                            | N/A |
| v       | Print all registers (V0-VF, I)                        | N/A |
| t       | Print both timers                                     | N/A |
| m       | Print address in memory to                            | Print the value in memory at a given address (hexadecimal) |
| i       | Print all breakpoints                                 | N/A |
| b       | Add a breakpoint                                      | Address to add (hexadecimal) |
| d       | Delete a breakpoint                                   | Address to delete (hexadecimal) |
| n       | Run just the next opcode                              | N/A |
| c       | Continue the program to the end or until a breakpoint | N/A |
| e       | exit the program                                      | N/A |


