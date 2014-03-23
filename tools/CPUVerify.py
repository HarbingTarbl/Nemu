#Verify 6502

import re
import pprint

ninReg = re.compile(r"^(\w{4})\s+((?:\w{2}\s)*(?:\w{2}))\s+(\*?\w{3})\s(?:.*)\sA\:(\w{2})\sX\:(\w{2})\sY\:(\w{2})\sP\:(\w{2})\sSP\:(\w{2})\sCYC\:\s*(\w+)\sSL\:(-?\w+)$")

def ninParser(line):
    r = re.match(ninReg, line)
    d = dict(
        PC = r.group(1),
        OP = r.group(3),
        A = r.group(4),
        X = r.group(5),
        Y = r.group(6),
        P = r.group(7),
        SP = r.group(8)
    )
    return d

nemuReg = re.compile(r"^(\w{4})\s([\w*]+)\sA\:(\w+)\sX\:(\w+)\sY\:(\w+)\sP\:(\w+)\sSP\:(\w+)$")
def nemuParser(line):
    r = re.match(nemuReg, line)
    d = dict(
        PC = r.group(1),
        OP = r.group(2),
        A = r.group(3),
        X = r.group(4),
        Y = r.group(5),
        P = r.group(6),
        SP = r.group(7)
    )
    return d

def compare(l, r):
    for k,v in l.items():
        if r[k].lower() != v.lower():
            return False
    return True

lhs = open("../working/nestest.log", "r")
rhs = open("../working/cpu.log", "r")
line = 0




with open("cpu_verify.log", "w") as fout:
    while True:
        l = lhs.readline()
        r = rhs.readline()

        print(l, file=fout)
        print(r, file=fout)
        print('-'*10, file=fout)

        line += 1
        if l == "" and r == "":
            break
        elif l == "":
            print("Complete")
            break
        elif r == "":
            print("Error")

        l = ninParser(l)
        r = nemuParser(r)
        if not compare(l, r):
            print("Error Found @ Line ", line)
            print(l, file=fout)
            print(r, file=fout)
            break

