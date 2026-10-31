# Configurations

These are the configuration settings for the Chip 8 emulator. Throughout the years there have been many different versions which change the way some instructions work. Depending on the game you are trying to run, you may have to change the settings.

This needs to be updated


## Changing the Configuration

When running the emulator or assembler, pass the desired configuration as a command line argument.

```
./Chip_8 program.ch8 -c chip-48
```

## Implemented Versions

Here is a list of the current supported implementations of the Chip 8.

* Chip 8
* Chip 48


## Quirks

Here are all the possible settings that each configuration could change.


| Macro | Instruction | Undefined | Defined |
|-------|-------------|---------|-------------|
| SHIFT | 8XY6, 8XYE | `VX = VY >> 1`, `VX = VY << 1` | `VX = VX >> 1`, `VX = VX << 1` |
| JUMP_OFFSET | BNNN | The address offset will be in V0 | The address offset will be in `VX` (BXNN) |
| REG_MEMORY_INDEX | FX55, FX65 | Index will update to `I + X + 1` by the end | Index will update to `I + X` by the end |
| BOOLEAN_RESET| 8XY1, 8XY2, 8XY3 | Will reset the VF register on boolean instructions | Will not reset the VF register on boolean instuctions |



## Quirks in Versions


| Version | CLI Flag | Quirks Defined |
|---------|----------|----------------|
| Chip 8  | chip-8   | N/A            |
| Chip 48 | chip-48  | SHIFT, JUMP_OFFSET, REG_MEMORY_INDEX, BOOLEAN_RESET |
