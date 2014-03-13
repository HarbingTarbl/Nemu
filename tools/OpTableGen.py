#Packgen
import os

opcodes = [
    ("ADC", [
        (0x69, "IMM", 2, 2),
        (0x65, "ZP", 2, 3),
        (0x75, "ZPX", 2, 4),
        (0x6D, "ABS", 3, 4),
        (0x7D, "ABX", 3, 4),
        (0x79, "ABY", 3, 4),
        (0x61, "IIX", 2, 6),
        (0x71, "IIY", 2, 5)
        ]
     ),
    ("AND", [
        (0x29, "IMM", 2, 2),
        (0x25, "ZP", 2, 3),
        (0x35, "ZPX", 2, 4),
        (0x2D, "ABS", 3, 4),
        (0x3D, "ABX", 3, 4),
        (0x39, "ABY", 3, 4),
        (0x21, "IIX", 2, 6),
        (0x31, "IIY", 2, 5)
        ]
     ),
    ("ASL", [
        (0x0A, "ACC", 1, 2),
        (0x06, "ZP", 2, 5),
        (0x16, "ZPX", 2, 6),
        (0x0E, "ABS", 3, 6),
        (0x1E, "ABX", 3, 7)
        ]
     ),
    ("BCC", [
        (0x90, "REL", 2, 2)
        ]
     ),
    ("BCS", [
        (0xB0, "REL", 2, 2)
        ]
     ),
    ("BEQ", [
        (0xF0, "REL", 2, 2),
        ]
     )
    ,
    ("BIT", [
        (0x24, "ZP", 2, 3),
        (0x2C, "ABS", 3, 4),
        ]
     ),
    ("BMI", [
        (0x30, "REL", 2, 2)
        ]
     ),
    ("BNE", [
        (0xD0, "REL", 2, 2)
        ]
     ),
    ("BPL", [
        (0x10, "REL", 2, 2)
        ]
     ),
    ("BRK", [
        (0x00, "IMP", 2, 2)
        ]
     ),
    ("BVC", [
        (0x50, "REL", 2, 2)
        ]
     ),
    ("BVS", [
        (0x70, "REL", 2, 2)
        ]
     ),
    ("CLC", [
        (0x18, "IMP", 1, 2)
        ]
     ),
    ("CLD", [
        (0xD8, "IMP", 1, 2)
        ]
     ),
    ("CLI", [
        (0x58, "IMP", 1, 2)
        ]
     ),
    ("CLV", [
        (0xB8, "IMP", 1, 2)
        ]
     ),
    ("CMP", [
        (0xC9, "IMM", 2, 2),
        (0xC5, "ZP", 2, 3),
        (0xD5, "ZPX", 2, 4),
        (0xCD, "ABS", 3, 4),
        (0xDD, "ABX", 3, 4),
        (0xD9, "ABY", 3, 4),
        (0xC1, "IIX", 2, 6),
        (0xD1, "IIY", 2, 5)
        ]
     ),
    ("CPX", [
        (0xE0, "IMM", 2, 2),
        (0xE4, "ZP", 2, 3),
        (0xEC, "ABS", 3, 4)
        ]
     ),
    ("CPY", [
        (0xC0, "IMM", 2, 2),
        (0xC4, "ZP", 2, 3),
        (0xCC, "ABS", 3, 4)
        ]
     ),
    ("DEC", [
        (0xC6, "ZP", 2, 5),
        (0xD6, "ZPX", 2, 6),
        (0xCE, "ABS", 3, 6),
        (0xD3, "ABX", 3, 7)
        ]
     ),
    ("DEX", [
        (0xCA, "IMP", 1, 2)
        ]
     ),
    ("DEY", [
        (0xC88, "IMP", 1, 2)
        ]
     ),
    ("EOR", [
        (0x49, "IMM", 2, 2),
        (0x45, "ZP", 2, 3),
        (0x55, "ZPX", 2, 4),
        (0x40, "ABS", 3, 4),
        (0x50, "ABX", 3, 4),
        (0x59, "ABY", 3, 4),
        (0x41, "IIX", 2, 6),
        (0x51, "IIY", 2, 5)
        ]
     ),
    ("INC", [
        (0xE6, "ZP", 2, 5),
        (0xF6, "ZPX", 2, 6),
        (0xEE, "ABS", 3, 6),
        (0xFE, "ABX", 3, 7)
        ]
     ),
    ("INX", [
        (0xE8, "IMP", 1, 2)
        ]
     ),
    ("INY", [
        (0xC8, "IMP", 1, 2)
        ]
     ),
    ("JMP", [
        (0x4C, "ABS", 3, 3),
        (0x6C, "IND", 3, 5)
        ]
     ),
    ("JSR", [
        (0x20, "ABS", 3, 6)
        ]
     ),
    ("LDA", [
        (0xA9, "IMM", 2, 2),
        (0xA5, "ZP", 2, 3),
        (0xB5, "ZPX", 2, 4),
        (0xAD, "ABS", 3, 4),
        (0xBD, "ABX", 3, 4),
        (0xB9, "ABY", 3, 4),
        (0xA1, "IIX", 2, 6),
        (0xB1, "IIY", 2, 5),
        ]
     ),
    ("LDX", [
        (0xA2, "IMM", 2, 2),
        (0xA6, "ZP", 2, 3),
        (0xB6, "ZPX", 2, 4),
        (0xAE, "ABS", 3, 4),
        (0xBE, "ABY", 3, 4),
        ]
     ),
    ("LDY", [
        (0xA0, "IMM", 2, 2),
        (0xA4, "ZP", 2, 3),
        (0xB4, "ZPX", 2, 4),
        (0xAC, "ABS", 3, 4),
        (0xBC, "ABX", 3, 4),
        ]
     ),
    ("LSR", [
        (0x4A, "ACC", 1, 2),
        (0x46, "ZP", 2, 5),
        (0x56, "ZPX", 2, 6),
        (0x4E, "ABS", 3, 6),
        (0x5E, "ABX", 3, 7),
        ]
     ),
    ("NOP", [
        (0xEA, "NOP", 1, 2),
        ]
     ),
    ("ORA", [
        (0x09, "IMM", 2, 2),
        (0x05, "ZP", 2, 3),
        (0x15, "ZPX", 2, 4),
        (0x0D, "ABS", 3, 4),
        (0x1D, "ABX", 3, 4),
        (0x19, "ABY", 3, 4),
        (0x01, "IIX", 2, 6),
        (0x11, "IIY", 2, 5),
        ]
     ),
    ("PHA", [
        (0x48, "IMP", 1, 3)
        ]
     ),
    ("PHP", [
        (0x08, "IMP", 1, 3)
        ]
     ),
    ("PLA", [
        (0x68, "IMP", 1, 4)
        ]
     ),
    ("PLP", [
        (0x28, "IMP", 1, 4),
        ]
     ),
    ("ROL", [
        (0x2A, "ACC", 1, 2),
        (0x26, "ZP", 2, 5),
        (0x36, "ZPX", 2, 6),
        (0x2E, "ABS", 3, 6),
        (0x3E, "ABX", 3, 7),
        ]
     ),
    ("ROR", [
        (0x6A, "ACC", 1, 2),
        (0x66, "ZP", 2, 5),
        (0x76, "ZPX", 2, 6),
        (0x6E, "ABS", 3, 6),
        (0x7E, "ABX", 3, 7),
        ]
     ),
    ("RTI", [
        (0x4D, "IMP", 1, 6),
        ]
     ),
    ("RTS", [
        (0x60, "IMP", 1, 6),
        ]
     ),
    ("SBC", [
        (0xE9, "IMM", 2, 2),
        (0xE5, "ZP", 2, 3),
        (0xF5, "ZPX", 2, 4),
        (0xED, "ABS", 3, 4),
        (0xFD, "ABX", 3, 4),
        (0xF9, "ABY", 3, 4),
        (0xE1, "IIX", 2, 6),
        (0xF1, "IIY", 2, 5)
        ]
     ),
    ("SEC", [
        (0x38, "IMP", 1, 2)
        ]
     ),
    ("SED", [
        (0xF8, "IMP", 1, 2)
        ]
     ),
    ("SEI", [
        (0x78, "IMP", 1, 2)
        ]
     ),    
# start of will's work
	("STA", [
		(0x85, "ZP", 2, 3),
		(0x95, "ZPX", 2, 4),
		(0x8D, "ABS", 3, 4),
		(0x9D, "ABX", 3, 5),
		(0x99, "ABY", 3, 5),
		(0x81, "IIX", 2, 6),
		(0x91, "IIY", 2, 6)
		]
	 ),
	("STX", [
		(0x86, "ZP", 2, 3),
		(0x96, "ZPY", 2, 4),
		(0x8E, "ABS", 3, 4)
		]
	 ),
	("STY", [
		(0x84, "ZP", 2, 3),
		(0x94, "ZPX", 2, 4),
		(0x8C, "ABS", 3, 4)
		]
	 ),
	("TAX", [
		(0xAA, "IMP", 1, 2)
		]
	 ),
	("TAY", [
		(0xA8, "IMP", 1, 2)
		]
	 ),
	("TSX", [
		(0xBA, "IMP", 1, 2)
		]
	 ),
	("TXA", [
		(0x8A, "IMP", 1, 2)
		]
	 ),
	("TXS", [
		(0x9A, "IMP", 1, 2)
		]
	 ),
	("TYA", [
		(0x98, "IMP", 1, 2)
		]
	 ),
]

table = dict()
fi = open("../src/InstructionTable.inc", "w")

for x in range(0, 255):
    table[x] = ("NOP", "NOP", "NOP")

for op in opcodes:
    for version in op[1]:
        table[version[0]] = (op[0], version[1])

fi.write("const InstructionPack Table[] = {\n")
for k,v in table.iteritems():
    fi.write("\t{ " + v[1] +  " , " + v[0] + " , NOP }, //" + hex(k) + "\n")
fi.write("};\n")
fi.close()



