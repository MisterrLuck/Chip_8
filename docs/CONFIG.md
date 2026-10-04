# Configurations

These are the configuration settings for the Chip 8 emulator. Throughout the years there have been many different versions which change the way some instructions work. Depending on the game you are trying to run, you may have to change the settings.


## Changing the Configuration

To change a configuration, add this line to the CMakeLists.txt file, replacing the macro name with the setting you want to set

```CMakeLists.txt
add_compile_definitions(MACRO_NAME)
```


## All Configuration Options

Here is a list of all the possible settings changes.


| Macro | Instruction | Defined | Not Defined |
|-------|-------------|---------|-------------|
| SHIFT | 8XY6, 8XYE | `VX = VY >> 1`, `VX = VY << 1` | `VX = VX >> 1`, `VX = VX << 1` |
| JUMP_OFFSET | BNNN | The address offset will be in V0 | The address offset will be in `VX` (BXNN) |
| FX1E_OVERFLOW | FX1E | `VF` will not change with overflow | `VF` will change with overflow |
| REG_MEMORY_INDEX | FX55, FX65 | Index will update to `I + X + 1` by the end | Index will not change |


When defined, the setting will be the original implementation. When not defined, the setting will be the newer implementation, usually from the CHIP-48 or SUPER-CHIP.


## Recomended Settings

```CMakeLists.txt
add_compile_definitions(JUMP_OFFSET)
```


## Notes

Most of these changes were changed with the CHIP-48 and the SUPER-CHIP.

Defining the macros will default the setting to its original implementation.


## Contributing

If you have any examples of which settings certain games need to run, please help out.
