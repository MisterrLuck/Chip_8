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


| Name | Affected Instructions | Off | On |
|-------|-------------|---------|-------------|
| shiftOnlyVX | 8XY6, 8XYE | Shift instructions take `VY` as the input and `VX` as the output | Shift instructions take `VX` as both the input and output |
| memoryIncrementByX | FX55, FX66 | Index register `I` will be incremented by `X + 1` by the end | Index register `I` will be incremented by `X` by the end |
| memoryLeaveIUnchanged | FX55, FX66 | Index register `I` will be incremented | Index register `I` will remain unchanged |
| drawWrap | DXYN | Sprites will be clipped if drawn on the edge | Sprites will wrap around to the other side when drawn on the edge |
| jumpOffsetVX | BNNN/BXNN | The address offset will be in `V0` (BNNN) | The address offset will be in `VX` (BXNN) |
| drawWait | DXYN, 00E0 | Sprites will always be drawn immediately | Only one sprite can be drawn per frame |
| bitwiseResetVF | 8XY1, 8XY2, 8XY3 | Will not reset the `VF` register on boolean instuctions | Will reset the `VF` register on boolean instructions |



## Quirks in Versions

Here are what quirks each version will change.


| Version | CLI Flag | Quirks Defined |
|---------|----------|----------------|
| Chip 8  | chip-8   | drawWait, bitwiseResetVF |
| Chip 48 | chip-48  | shiftOnlyVX, memoryIncrementByX, jumpOffsetVX |

