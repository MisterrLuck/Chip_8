# Assembler

This is the custom assembly language that I wrote. It is decently simplistic.

> Also refer to the `example_assembly.asm`.

## Parameter

### Dynamic Parameters

N is a 4 bit unsigned integer.
NN is an 8 bit unsigned integer.
LABEL is a label defined somewhere else in the program.
VX and VY are general purpose registers defined as V0 - VF.

### Static Parameters

I is the index register defined as I.
DT is the delay timer defined as DT.
ST is the sound timer defined as ST.


## Instructions

Define labels with a name followed by a colon.

```asm
label_name:
```


| Instruction | Description | Parameters |
|-------------|-------------|------------|
| CLS         | Clears the Screen | N/A |
| RET         | Returns from a subrouting | N/A |
| JUMP        | Jump to a label | LABEL |
| CALL        | Call a subroutine at a label | LABEL |
| STOR        | Store a number into the left register | (VX, NN), (VX, VY), (I, LABEL), (VX, DT), (DT, VX), (ST, VX) |
| ADD         | Add two numbers and store in the left register | (VX, NN), (VX, VY), (I, VX) | 
| SUBR        | Subtract the right from the left, and store in the left register | VX, VY  |
| SUBL        | Subtract the left from the right, and store in the left register | VX, VY  |
| SKE         | Skip next instruction if the two numbers are equal | (VX, NN), (VX, VY) |
| SKE         | Skip next instruction if the two numbers are not equal | (VX, NN), (VX, VY) |
| OR          | Bitwise OR between two numbers, store in the left register |  VX, VY  |
| AND         | Bitwise AND between two numbers, store in the left register | VX, VY  |
| XOR         | Bitwise XOR between two numbers, store in the left register | VX, VY  |
| JUMPO       | Jump to label with an offset of V0 bytes | LABEL |
| SKK         | Skip next instruction if the key stored in VX has been pressed | VX |
| SKK         | Skip next instruction if the key stored in VX has not been pressed | VX |
| RAND        | Generate a random 8 bit number bitwise AND with VX | VX |
| DRAW        | Draw a sprite located at the Index register, at coordinates (VX, VY), with a height of N | VX, VY, N |
| KEY         | Wait until a key, VX, has been pressed | VX |
| CHAR        | Get the font sprite for the letter in VX | VX |
| BCD         | Store the value in VX as Binary Coded Decimal at the location in the Index register | VX |
| SMEM        | Store the data at the Index register location into register V0 - VX | VX |
| LMEM        | Load the data from registers V0 - VX into memory at the location in the Index register | VX |
| BYTES       | Store sprite data into memory directly at the current location | NN, ... |



## Instructions Based on Configuration

These opcodes depend on the [settings you have configured](configuration.md). 


| Instruction | Description | Parameters |
|-------------|-------------|------------|
| SHR         | Shift the VY register right by one, and store in VX | VX, VY |
| SHR         | Shift the VX register right by one | VX |
| SHL         | Shift the VY register left by one, and store in VX | VX, VY |
| SHL         | Shift the VX register right by one | VX |


