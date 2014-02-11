#Packgen
import os

opcodes = [
    ("ADC", [
        (0x69, "IMM"),
        (0x65, "ZP"),
        (0x75, "ZPX"),
        (0x60, "ABS"),
        (0x70, "ABX"),
        (0x79, "ABY"),
        (0x61, "IIX"),
        (0x71, "IIY")
        ]
     )
]

table = dict()
fi = open("../src/InstructionTable.inc", "w")

for x in range(0, 255):
    table[x] = ("NOP", "NOP", "NOP")

for op in opcodes:
    for version in op[1]:
        table[version[0]] = (op[0], version[1])

fi.write("const InstructionPack const Table[] = {\n")
for k,v in table.iteritems():
    fi.write("\t{ " + v[1] +  " , " + v[0] + " , NOP }, //" + hex(k) + "\n")
fi.write("};\n")
fi.close()



