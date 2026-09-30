print("Welcome to the Code To ROM Helper!")

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
code = code.replace(" ", "")
code = code.replace("\n", "")

# Convert to bytes and write to the file
b = bytes.fromhex(code)
print(b.hex())

with open(out_file, "wb") as binary_file:
    binary_file.write(b)

