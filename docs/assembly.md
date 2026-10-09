
Here is the assembly language


| Opcode | Mnemonic | Pseudo Code |
|------|-----|---|
| 00E0 | CLS          | |
| 00EE | RET          | |
| 1NNN | JMP   NNN    | |
| 2NNN | CALL  NNN    | |
| 3XNN | SKE   VX, NN | |
| 4XNN | SKNE  VX, NN | |
| 5XY0 | SKE   VX, VY | |
| 6XNN | STR   VX, NN | VX = NN |
| 7XNN | ADD   VX, NN | VX = VX + NN |
| 8XY0 | STR   VX, VY | VX = VY |
| 8XY1 | OR    VX, VY | VX = VX OR VY| 
| 8XY2 | AND   VX, VY | VX = VX AND VY|
| 8XY3 | XOR   VX, VY | VX = VX XOR VY| 
| 8XY4 | ADD   VX, VY | VX = VX + VY |
| 8XY5 | SUBR  VX, VY | VX = VX - VY |
| 8XY7 | SUBL  VX, VY | VX = VY - VX |
| 9XY0 | SKNE  VX, VY | |
| ANNN | STR   I, NNN | I = NNN |
| CXNN | RND   VX, NN | VX = rand(NN)| 
| DXYN | DRAW  VX, VY | |
| EX9E | SKK   VX     | |
| EXA1 | SKNK  VX     | |
| FX07 | STR   VX, DT | VX = DT |
| FX0A | KEY   VX     | |
| FX15 | STR   DT, VX | DT = VX |
| FX18 | STR   ST, VX | ST = VX |
| FX1E | ADD   I, VX  | I = I + VX |
| FX29 | CHAR  VX     | |
| FX33 | BCD   VX     | |
| FX55 | SMEM  VX     | |
| FX65 | LMEM  VX     | | 


These opcodes depend on the [settings you have configured](configuration.md). 

| Opcode | Mnemonic | Pseudo Code |
|------|-----|---|
| 8XY6 | SHR  VX, VY | VX = VY >> 1 | 
| 8XY6 | SHR  VX     | VX = VX >> 1 | 
| 8XYE | SHL  VX, VY | VX = VY << 1 |
| 8XYE | SHL  VX     | VX = VX << 1 |
| BNNN | JMPO  VX, NNN| JMP VX + NNN |
| BNNN | JMPO  NNN    | JMP V0 + NNN |


