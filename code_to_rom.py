# TODO: add arguments bc its better
import sys

print("Welcome to the Code To ROM Helper!")

if len(sys.argv) > 2:
    inp_file = sys.argv[1]
    out_file = sys.argv[2]
elif len(sys.argv) == 2:
    inp_file = sys.argv[1]
    print(inp_file)
    if inp_file[-4:] == ".txt":
        out_file = inp_file[:-4] + ".ch8"
    else:
        out_file = inp_file + ".ch8"

else:
    print("\nEnter your filename")
    inp_file = input(">> ")

    print("\nEnter you desired filename")
    out_file = input(">> ")


# Ensure its a chip 8 file
if out_file[-4:] != ".ch8":
    out_file += ".ch8"


# Read the code in 
code = ""
with open(inp_file, "r") as file:
    code = file.read()

# Processing
## Removing comments
lines = code.split("\n")
code = ""
for line in lines:
    if "#" in line:
        line = line[:line.index("#")]
    code += line

## Remove whitespace and newlines
code = code.replace(" ", "")
code = code.replace("\n", "")

# Convert to bytes and write to the file
b = bytes.fromhex(code)

with open(out_file, "wb") as binary_file:
    binary_file.write(b)

print(f"Written to output file: {out_file}")
