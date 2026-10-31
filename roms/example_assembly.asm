JUMP start

start:
	# Make comments with `#`
	; Or even with `;`
	CLS

	STOR V0, 0x10
	# Register names have to be capitalized
	JUMP label

func:
	RAND V1, 15 # Can differentiate between hex and decimal
	RET

label:
	CALL func
	ADD I, V0
	# ST is the sound timer
	# DT is the delay timer
	STOR ST, V5
	STOR V5, DT

	; Any opcode with NNN can be replaced with a label
	# Can still use direct address probably?
	STOR I, data_1

data_1:
	BYTES 0b10011010 # Can also differentiate between binary
	# Bytes can be separated by a comma
	BYTES 0b01101010, 0b01010111

data_2:
	BYTES 0xF
	BYTES 10

# Different from data_2
DATA_2:
	BYTES 0x5
	BYTES 0x7
