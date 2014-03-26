#HashTest

_author_ = "HarbingTarbl"

def get_section_normal(number):
	if number >= 0xC000:
		return "Upper PRG"
	elif number >= 0x8000:
		return "Lower PRG"
	elif number >= 0x6000:
		return "SRAM"
	elif number >= 0x4020:
		return "EX-ROM"
	elif number >= 0x4000:
		return "APU"
	elif number >= 0x2000:
		return "PPU"
	else:
		return "RAM"

sectionList = [ 
	lambda x: "RAM", 
	lambda x: "PPU",
	lambda x: "EX-ROM" if x >= 0x4020 else "APU",
	lambda x: "SRAM", 
	lambda x: "Lower PRG", 
	lambda x: "Lower PRG",
	lambda x: "Upper PRG",
	lambda x: "Upper PRG",
	]		

def get_section_hash(number):
	indx = (number >> 13) & 0x07
	#print(hex(number) , " @ " , indx)
	return sectionList[indx](number)
	

with open("hash.log", "w") as fhash:
	with open("normal.log", "w") as fnormal:
		for addr in range(0, 0x10000):
			hashResult = get_section_hash(addr)
			normalResult = get_section_normal(addr)
			fhash.write(hashResult + '\n')
			fnormal.write(normalResult + '\n')
			if hashResult != normalResult:
				print("Bad result @ ", hex(addr), "\n", hashResult, " : ", normalResult)
				exit(0)

from random import randint
from timeit import repeat as timeit

def test_function(func):
	return lambda: func(randint(0, 0xFFFF))
		


print(sum(timeit(stmt=test_function(get_section_hash)))/3)
print(sum(timeit(stmt=test_function(get_section_normal)))/3)
	
	