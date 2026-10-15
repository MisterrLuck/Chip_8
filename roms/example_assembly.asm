global start

start:
	CLS

	# Adding comments
	STOR V0, 0x10
	# Register names have to be capitalized
	JUMP label

func:
	RAND V1, 15 # Can differentiate between hex and decimal
	RET

label:
	CALL func
	add I, V0
	# ST is the sound timer
	# DT is the delay timer
	STOR ST, 0x60
	stor V5, DT

	# Any opcode with NNN can be replaced with a label
	# Can still use direct address probably?
	STOR I, data_1

data_1:
	BYTE 0b10011010 # Can also differentiate between binary
	BYTE 0b01101010
	BYTE 0b01010111

data_2:
	# Capitalization doesn't matter except for labels
	byte 0xF
	ByTe 10

# Different from data_2
DATA_2:
	BYTE 0x5
	BYTE 0x7
