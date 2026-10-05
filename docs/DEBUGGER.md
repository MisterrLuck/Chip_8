# Debugger

The custom debugger will allow instructions run one by one.


## Usage

Add this line to the CMakeLists.txt to run the program with the debugger

```CMakeLists.txt
add_compile_definitions(DEBUGGER)
```


## List of commands

You can only enter one character for commands.


| Command | Description | 
|---------|-------------|
| n | run the next opcode |
| l | print opcode that just ran |
| o | print next opcode |
| v | print registers |
| i | print index |
| d | print delay timer |
| s | print sound timer |
| c | continue the program to the end |
| e | exit the program | 

