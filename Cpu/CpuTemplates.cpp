#include "Cpu.h"

#define NUMBER cpu::MemoryType::NUMBER
#define REGISTER cpu::MemoryType::REGISTER
#define MEMORY cpu::MemoryType::MEMORY
#define FLAG cpu::MemoryType::FLAG
#define GRAPHICS cpu::MemoryType::GRAPHICS
#define NOTHING cpu::MemoryType::NOTHING

using namespace cpu;

CPU_T
inline void CpuSimulator<cpu_t>::initMaps() {
	typedef std::pair<MemoryType, MemoryTypeRange<cpu_t>> rangePair;
	typedef std::pair<std::tuple<std::string, MemoryType>, MemoryTypeRange<cpu_t> > bruhRange;
	typedef std::pair<std::tuple<std::string, MemoryType, MemoryType>, MemoryTypeRange<cpu_t> > bruhRange2;

	cpu_t number = std::numeric_limits<cpu_t>::max()-1;
	printf("number: %d\n", number);

	rangesMap.insert(rangePair{ NUMBER, {-number, number} });
	rangesMap.insert(rangePair{ REGISTER, {0,REGISTER_SIZE-1} });
	rangesMap.insert(rangePair{ MEMORY, {0,MEMORY_SIZE-1} });
	rangesMap.insert(rangePair{ FLAG, {0,FLAGS-1} });
	rangesMap.insert(rangePair{ GRAPHICS, {0,GRAPHICS_SIZE-1} });//TODO i havent implemented yet

	bruhMap.insert(bruhRange{ {"jmp", NUMBER}, {0, number} });
	bruhMap.insert(bruhRange{ {"jgt", NUMBER}, {0, number} });
	bruhMap.insert(bruhRange{ {"jngt", NUMBER}, {0, number} });
	bruhMap.insert(bruhRange{ {"je", NUMBER}, {0, number} });
	bruhMap.insert(bruhRange{ {"jne", NUMBER}, {0, number} });

	//mov gp 0-number
	bruhMap2.insert(bruhRange2{ {"mov", GRAPHICS, NUMBER}, {0, number} });
}

CPU_T
CpuSimulator<cpu_t>::CpuSimulator() {
	cpu::initMaps();
	initMaps();
}

CPU_T
void CpuSimulator<cpu_t>::dumpRegisters() {
	for (cpu_t i = 0; i < REGISTER_SIZE; i++) {
		printf("r%d: %d\n", i, registers[i]);
	}
}

CPU_T
void CpuSimulator<cpu_t>::resetRegisters() {
	for (cpu_t i = 0; i < REGISTER_SIZE; i++) {
		registers[i] = 0;
	}
}

CPU_T
void CpuSimulator<cpu_t>::resetMemory() {
	for (cpu_t i = 0; i < MEMORY_SIZE; i++) {
		memory[i] = 0;
	}
}

CPU_T
cpu_t CpuSimulator<cpu_t>::getRegister(unsigned char r) {
	//TODO: fix this one and getMemory I just cant figure out how to throw excpeptions lol
	//if (r > 15 || r < 0) throw out_of_range("");
	return registers[r];
}

CPU_T
cpu_t CpuSimulator<cpu_t>::getMemory(unsigned short m) {
	//if (m > (0xFFFF - 1) || m < 0) throw out_of_range();
	return memory[m];
}

CPU_T
void CpuSimulator<cpu_t>::dumpFlags() {
	for (cpu_t i = 0; i < FLAGS; i++) {
		printf("f%d: %d\n", i, flags[i]);
	}
}

CPU_T
void CpuSimulator<cpu_t>::dumpPointers() {
	printf("ip: %d\n", ip);
	printf("gp: %d\n", gp);
}

CPU_T
bool CpuSimulator<cpu_t>::setRegister(unsigned char r, size_t value) {
	if (r > REGISTER_SIZE-1 || r < 0) return false;//bound check
	registers[r] = value;
	return true;
}

CPU_T
bool CpuSimulator<cpu_t>::setMemory(unsigned short m, size_t value) {
	if (m > (MEMORY_SIZE-1) || m < 0) return false;//bound check
	memory[m] = value;
	return true;
}

CPU_T
void CpuSimulator<cpu_t>::resetFlags() {
	for (cpu_t i = 0; i < FLAGS; i++) {
		flags[i] = 0;
	}
}

CPU_T
char CpuSimulator<cpu_t>::getOpSize() {
	return 2 * sizeof(cpu_t) + 1;
}

CPU_T
void CpuSimulator<cpu_t>::runProgram(byte *code, int progLength) {
	resetRegisters();
	resetFlags();
	if (verbose) {
			printf("test\n");
			printf("code[0] = %d\n", code[0]);
			printf("opSize = %d\n", getOpSize());
	}
	for (ip = 0; ip < progLength; ip += getOpSize()) {
		std::vector<byte> op(getOpSize(), 0);
		for (int j = 0; j < getOpSize(); j++) {
			op[j] = code[ip + j];
		}

		//if (verbose)printBitArray(code, getOpSize(), 0);
		executeOp(op);
	}
}

CPU_T
bool CpuSimulator<cpu_t>::executeOp(cpu_t op, cpu_t left, cpu_t right) {
	if (verbose) {
		printf("executed op:%d, left: %d, right: %d\n", op, left, right);
		printf("ip: %d\n", ip);
		//printf("left: "); bin(left); printf("\nright: "); bin(right); printf("\n");
	}
	switch (op) {
	case 0x00://mov register number
		registers[left] = right;
		if (verbose)printf("mov %d to r%d\n", right, left);
		break;
	case 0x01://mov register register
		registers[left] = registers[right];
		break;
	case 0x02://mov register ValueAtMemory
		registers[left] = memory[right];
		break;
	case 0x03://mov register ValueAtMemory
		registers[left] = flags[right];
		break;
	case 0x04://mov register ValueAtMemory
		registers[left] = gp;
		break;

	case 0x05://mov memory number
		memory[left] = right;
		break;
	case 0x06://mov memory register
		memory[left] = registers[right];
		break;
	case 0x07://mov memory ValueAtMemory
		memory[left] = memory[right];
		break;
	case 0x08://mov memory flag
		memory[left] = flags[right];
		break;
	case 0x09://mov memory flag
		memory[left] = gp;
		break;

	case 0x0A://mov gp number
		gp = right;
		break;
	case 0x0B://mov gp register
		gp = registers[right];
		break;
	case 0x0C://mov gp ValueAtMemory
		gp = memory[right];
		break;
	case 0x0D://mov gp flag
		gp = flags[right];
		break;
	case 0x0E://mov gp naaaaannniiIIIII!!!!!
		gp = gp;//ecks dee!?!?!??!
		break;



	case 0x10://add register number
		registers[left] += right;
		break;
	case 0x11://add register register
		registers[left] += registers[right];
		break;
	case 0x12://add register valueAtMemory
		registers[left] += memory[right];
		break;
	case 0x13://add register valueAtMemory
		registers[left] += flags[right];
		break;

	case 0x14://add memory number
		memory[left] += right;
		break;
	case 0x15://add memory register
		memory[left] += registers[right];
		break;
	case 0x16://add memory valueAtMemory
		memory[left] += memory[right];
		break;
	case 0x17://add memory valueAtMemory
		memory[left] += flags[right];
		break;

	case 0x18://add memory number
		gp += right;
		break;
	case 0x19://add memory register
		gp += registers[right];
		break;
	case 0x1A://add memory valueAtMemory
		gp += memory[right];
		break;
	case 0x1B://add memory valueAtMemory
		gp += flags[right];
		break;



	case 0x20://sub register number
		registers[left] -= right;
		if (verbose)printf("sub r%d %d\nr%d = %d\n", left, right, left, registers[left]);
		break;
	case 0x21://sub register register
		registers[left] -= registers[right];
		break;
	case 0x22://sub register valueAtMemory
		registers[left] -= memory[right];
		break;
	case 0x23://sub register valueAtMemory
		registers[left] -= flags[right];
		break;

	case 0x24://sub memory number
		memory[left] -= right;
		break;
	case 0x25://sub memory register
		memory[left] -= registers[right];
		break;
	case 0x26://sub memory valueAtMemory
		memory[left] -= memory[right];
		break;
	case 0x27://sub memory valueAtMemory
		memory[left] -= flags[right];
		break;

	case 0x28://sub gp number
		gp -= right;
		break;
	case 0x29://sub gp register
		gp -= registers[right];
		break;
	case 0x2A://sub gp valueAtMemory
		gp -= memory[right];
		break;
	case 0x2B://sub gp valueAtMemory
		gp -= flags[right];
		break;

		//branchless coding ftw
	case 0x30://jump location
		ip = jmp_range_check(left) - getOpSize();
		if (verbose)printf("jmp to %d\n", left);
		break;
	case 0x31://jumpIfGT location
		ip = (flags[0] * (jmp_range_check(left) - getOpSize())) + (!flags[0] * (unsigned long)ip);
		break;
	case 0x32://jumpIfNotGT location
		ip = (!flags[0] * (jmp_range_check(left) - getOpSize())) + (flags[0] * (unsigned long)ip);
		break;
	case 0x33://jumpIfEqual location
		ip = (flags[1] * (jmp_range_check(left) - getOpSize())) + (!flags[1] * (unsigned long)ip);
		break;
	case 0x34://jumpIfNotEqual location
		ip = (!flags[1] * (jmp_range_check(left) - getOpSize())) + (flags[1] * (unsigned long)ip);
		break;
	case 0x35://jumpIfNotZero location
		ip = (!flags[1] * (jmp_range_check(left) - getOpSize())) + (flags[1] * (unsigned long)ip);
		break;
	case 0x36://jump register
		ip = jmp_range_check(registers[left]) - getOpSize();
		break;
	case 0x37:
		//nop
		break;


	case 0x40://multiply register number
		registers[left] *= right;
		break;
	case 0x41://multiply register register
		registers[left] *= registers[right];
		break;
	case 0x42://multiply register valueAtMemory
		registers[left] *= memory[right];
		break;
	case 0x43://multiply register valueAtMemory
		registers[left] *= flags[right];
		break;

	case 0x44://multiply memory number
		memory[left] *= right;
		break;
	case 0x45://multiply memory register
		memory[left] *= registers[right];
		break;
	case 0x46://multiply memory valueAtMemory
		memory[left] *= memory[right];
		break;
	case 0x47://multiply memory valueAtMemory
		memory[left] *= flags[right];
		break;

	case 0x48://multiply memory number
		gp *= right;
		break;
	case 0x49://multiply memory register
		gp *= registers[right];
		break;
	case 0x4A://multiply memory valueAtMemory
		gp *= memory[right];
		break;
	case 0x4B://multiply memory valueAtMemory
		gp *= flags[right];
		break;


	case 0x50://compare register number
		flags[0] = registers[left] > right;
		flags[1] = registers[left] == right;
		break;
	case 0x51://compare register register
		flags[0] = registers[left] > registers[right];
		flags[1] = registers[left] == registers[right];
		break;
	case 0x52://compare register memory
		flags[0] = registers[left] > memory[right];
		flags[1] = registers[left] == memory[right];
		break;
	case 0x53://compare register flag
		flags[0] = registers[left] > flags[right];
		flags[1] = registers[left] == flags[right];
		break;

	case 0x54://compare memory number
		flags[0] = memory[left] > right;
		flags[1] = memory[left] == right;
		break;
	case 0x55://compare memory register
		flags[0] = memory[left] > registers[right];
		flags[1] = memory[left] == registers[right];
		break;
	case 0x56://compare memory memory
		flags[0] = memory[left] > memory[right];
		flags[1] = memory[left] == memory[right];
		break;
	case 0x57://compare memory flag
		flags[0] = memory[left] > flags[right];
		flags[1] = memory[left] == flags[right];
		break;

	case 0x58://compare memory number
		flags[0] = gp > right;
		flags[1] = gp == right;
		break;
	case 0x59://compare memory register
		flags[0] = gp > registers[right];
		flags[1] = gp == registers[right];
		break;
	case 0x5A://compare memory memory
		flags[0] = gp > memory[right];
		flags[1] = gp == memory[right];
		break;
	case 0x5B://compare memory flag
		flags[0] = gp > flags[right];
		flags[1] = gp == flags[right];
		break;


	case 0x60://OR register number
		registers[left] |= right;
		break;
	case 0x61://OR register register
		registers[left] |= registers[right];
		break;
	case 0x62://OR register valueAtMemory
		registers[left] |= memory[right];
		break;
	case 0x63://OR register valueAtMemory
		registers[left] |= flags[right];
		break;

	case 0x64://OR memory number
		memory[left] |= right;
		break;
	case 0x65://OR memory register
		memory[left] |= registers[right];
		break;
	case 0x66://OR memory valueAtMemory
		memory[left] |= memory[right];
		break;
	case 0x67://OR memory valueAtMemory
		memory[left] |= flags[right];
		break;

	case 0x68://OR gp number
		gp |= right;
		break;
	case 0x69://OR gp register
		gp |= registers[right];
		break;
	case 0x6A://OR gp valueAtMemory
		gp |= memory[right];
		break;
	case 0x6B://OR gp valueAtMemory
		gp |= flags[right];
		break;


	case 0x70://AND register number
		registers[left] &= right;
		break;
	case 0x71://AND register register
		registers[left] &= registers[right];
		break;
	case 0x72://AND register valueAtMemory
		registers[left] &= memory[right];
		break;
	case 0x73://AND register valueAtMemory
		registers[left] &= flags[right];
		break;

	case 0x74://AND memory number
		memory[left] &= right;
		break;
	case 0x75://AND memory register
		memory[left] &= registers[right];
		break;
	case 0x76://AND memory valueAtMemory
		memory[left] &= memory[right];
		break;
	case 0x77://AND memory valueAtMemory
		memory[left] &= flags[right];
		break;

	case 0x78://AND gp number
		gp &= right;
		break;
	case 0x79://AND gp register
		gp &= registers[right];
		break;
	case 0x7A://AND gp valueAtMemory
		gp &= memory[right];
		break;
	case 0x7B://AND gp valueAtMemory
		gp &= flags[right];
		break;



	case 0x80://XOR register number
		registers[left] ^= right;
		break;
	case 0x81://XOR register register
		registers[left] ^= registers[right];
		break;
	case 0x82://XOR register valueAtMemory
		registers[left] ^= memory[right];
		break;
	case 0x83://XOR register valueAtMemory
		registers[left] ^= flags[right];
		break;

	case 0x84://XOR memory number
		memory[left] ^= right;
		break;
	case 0x85://XOR memory register
		memory[left] ^= registers[right];
		break;
	case 0x86://XOR memory valueAtMemory
		memory[left] ^= memory[right];
		break;
	case 0x87://XOR memory valueAtMemory
		memory[left] ^= flags[right];
		break;

	case 0x88://XOR gp number
		gp ^= right;
		break;
	case 0x89://XOR gp register
		gp ^= registers[right];
		break;
	case 0x8A://XOR gp valueAtMemory
		gp ^= memory[right];
		break;
	case 0x8B://XOR gp valueAtMemory
		gp ^= flags[right];
		break;



	case 0x90://NOT register 
		registers[left] = ~registers[left];
		break;
	case 0x91://NOT memory
		memory[left] = ~memory[left];
		break;
	case 0x92://NOT flag
		flags[left] = ~flags[left];
		break;
	case 0x93://NOT gp
		gp = ~gp;
		break;


	case 0xA0://shiftLeft register number
		registers[left] << right;
		break;
	case 0xA1://shiftLeft register register
		registers[left] << registers[right];
		break;
	case 0xA2://shiftLeft register valueAtMemory
		registers[left] << memory[right];
		break;
	case 0xA3://shiftLeft register valueAtMemory
		registers[left] << flags[right];
		break;

	case 0xA4://shiftLeft memory number
		memory[left] << right;
		break;
	case 0xA5://shiftLeft memory register
		memory[left] << registers[right];
		break;
	case 0xA6://shiftLeft memory valueAtMemory
		memory[left] << memory[right];
		break;
	case 0xA7://shiftLeft memory valueAtMemory
		memory[left] << flags[right];
		break;

	case 0xA8://shiftLeft gp number
		gp << right;
		break;
	case 0xA9://shiftLeft gp register
		gp << registers[right];
		break;
	case 0xAA://shiftLeft gp valueAtMemory
		gp << memory[right];
		break;
	case 0xAB://shiftLeft gp valueAtMemory
		gp << flags[right];
		break;


	case 0xB0://shiftRight register number
		registers[left] >> right;
		break;
	case 0xB1://shiftRight register register
		registers[left] >> registers[right];
		break;
	case 0xB2://shiftRight register valueAtMemory
		registers[left] >> memory[right];
		break;
	case 0xB3://shiftRight register valueAtMemory
		registers[left] >> flags[right];
		break;

	case 0xB4://shiftRight memory number
		memory[left] >> right;
		break;
	case 0xB5://shiftRight memory register
		memory[left] >> registers[right];
		break;
	case 0xB6://shiftRight memory valueAtMemory
		memory[left] >> memory[right];
		break;
	case 0xB7://shiftRight memory valueAtMemory
		memory[left] >> flags[right];
		break;

	case 0xB8://shiftRight gp number
		gp >> right;
		break;
	case 0xB9://shiftRight gp register
		gp >> registers[right];
		break;
	case 0xBA://shiftRight gp valueAtMemory
		gp >> memory[right];
		break;
	case 0xBB://shiftRight gp valueAtMemory
		gp >> flags[right];
		break;


	case 0xC0://gmov number
		graphics[gp] = left;
		break;
	case 0xC1://gmov register
		graphics[gp] = registers[left];
		break;
	case 0xC2://gmov valueAtMemory
		graphics[gp] = memory[left];
		break;
	case 0xC3://gmov flag
		graphics[gp] = flags[left];
		break;
	case 0xC4://gmov gp
		graphics[gp] = gp;//why lol
		break;

	default:
		if (verbose)printf("unknown op '%c'\n", op);
		return false;
		break;
	}
	return true;
}

//always round down
//and make sure its greater than zero
//ill look back at this and not understand a single word of it
CPU_T
cpu_t CpuSimulator<cpu_t>::jmp_range_check(cpu_t left) {
	return max(0,getOpSize() * ((int)((float)left/(float)getOpSize())));
}

CPU_T
bool CpuSimulator<cpu_t>::executeOp(std::vector<byte> code) {
	if (step)getchar();
	cpu_t op = code[0];
	cpu_t left;
	cpu_t right;

	char bytes = (getOpSize() - 1) / 2;

	for (int byte = bytes; byte > 0; byte--) {
		left = (left << 8) | code[byte];
	}
	for (int byte = bytes; byte > 0; byte--) {
		right = (right << 8) | code[bytes + byte];
	}
	return executeOp(op, left, right);
}

CPU_T
int CpuSimulator<cpu_t>::assemble(byte* prog, std::string path) {
	if(verbose)printf("trying open bruh\n");
	std::string str;
	std::ifstream file;
	file.open(path.c_str(), std::ios::in);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			str.append(line + "\n");
		}
		file.close();
		if (verbose)printf("%s\n", str.c_str());
	}
	//str.append(" ");
	if (verbose)printf("closing file\n\n");
	return assemble(prog, str.c_str(), str.size());
}

CPU_T
int CpuSimulator<cpu_t>::assemble(byte* program, const char str[], int strLength) {
	std::map<std::string, int> labels;
	cpu_t currentOp[3];//ops can only be 3 max lolll
	std::string identifier("");
	int programI = 0;
	int opI = 0;
	MemoryType left;
	MemoryType right;

	bool isLabel = false;
	bool preParse = true;
	bool isComment = false;
	bool isMacro = false;
	bool applyingMacro = false;//to be implemented
	std::string macroName;
	std::string macroBody;
	int macroIndex = 0;

	if (verbose)printf("PREPARSE\n");
	for (int i = 0; i < strLength; i++) {
		const char c = str[i];
		//if (c == ';')isComment = true;
		isComment = (c == ';');
		if (c == '\n' || c == EOF) {
			isComment = false;
			if (isMacro && false) {
				isMacro = false;
				macroMap.insert({ std::string(macroName) + '\0',
					std::string(macroBody) + '\0' });
				if (verbose)printf("macroBody: '%s'\n", macroBody.c_str());
			}
		}else if (isMacro && false) {
			macroBody += c;
			continue;
		}

		if (isspace(c) || c == '\0' || c == EOF) {//then make new thing or something idk
			if (c == EOF && preParse) {
				printf("going to phase 2\n");
				goto actualParse;
			} else if (
				(!(preParse && i == (strLength - 1))
					&& (removeSpace(identifier).empty()
						|| identifier.empty())) || isComment)
				continue;
			//identifier += '\0';

			//char *lastChar = identifier.back();
			//printf("identifier.end() : %c\n", lastChar);

			if(verbose)printf("identifier size: %d\n", (int)strlen(identifier.c_str()));
			if (strlen(identifier.c_str()) > 0){
				isLabel = identifier[strlen(identifier.c_str()) - 1] == ':';
			} else {
				goto actualParse;
			}

			if (macroMap.count(identifier) == 1) {
				//do the thing where you 
				if (verbose)printf("applied macro '%s', ", identifier.c_str());
				identifier = std::string(macroMap.at(identifier));
				if (verbose)printf("macroBody: '%s'\n", identifier.c_str());
			}

			if (verbose)printf("identifier: '%s', opI: %d, programI: %d\n", identifier.c_str(), opI, programI);

			if ( isLabel && opI != 0) {
				if (labels.count(identifier) == 1) {
					//currentOp[opI] = labels.at(identifier);
					identifier = std::to_string(labels.at(identifier));
					if (verbose)printf("replaced label with %s\n", identifier.c_str());
					currentOp[opI] = atoi(identifier.c_str());
					if (opI == 1) {
						left = NUMBER;
					}
					else if (opI == 2) {
						right = NUMBER;
					}
				}
				else if(!preParse){
					if (verbose)printf("label %s doesnt exist\n", identifier.c_str());
					return 0;
				}
			}
			else if ( identifier[0] == 'r' && opI != 0) {//tis but a register
				currentOp[opI] = atoi(identifier.substr(1, identifier.size() - 1).c_str());//ecks dee
				if (verbose)printf("%s -> register %d\n", identifier.c_str(), currentOp[opI]);
				if (opI == 1) {
					left = REGISTER;
				}
				else if (opI == 2) {
					right = REGISTER;
				}

			}
			else if ( identifier[0] == 'f' && opI != 0) {//tis but a flag
				currentOp[opI] = atoi(identifier.substr(1, identifier.size() - 1).c_str());//ecks dee
				if (verbose)printf("%s -> flag %d\n", identifier.c_str(), currentOp[opI]);
				if (opI == 1) {
					left = FLAG;
				}
				else if (opI == 2) {
					right = FLAG;
				}
			}
			else if ( identifier[0] == 'm' && opI != 0) {//tis but a memory locationN
				currentOp[opI] = atoi(identifier.substr(1, identifier.size() - 1).c_str());//ecks dee
				if (verbose)printf("%s -> memory %d\n", identifier.c_str(), currentOp[opI]);
				if (opI == 1) {
					left = MEMORY;
				}
				else if (opI == 2) {
					right = MEMORY;
				}
			}
			//tis but the graphics pointer
			else if ( identifier == "gp" && opI != 0) {
				if (verbose)printf("graphics pointer\n");
				if (opI == 1) {
					left = GRAPHICS;
				}
				else if (opI == 2) {
					right = GRAPHICS;
				}
			}
			//!preParse && 
			else if (isInteger(identifier.c_str())) {
				int num = stol(identifier);
				if (num > std::numeric_limits<cpu_t>::max()) {
					if (verbose)printf("number %d is too big, bigger than %d\n", num, sizeof(cpu_t));
					return 0;
				}
				else {
					currentOp[opI] = num;
					if (verbose)printf("currentOp[%d] = %d\n", opI, num);
					if (opI == 1) {
						left = NUMBER;
					}
					else if (opI == 2) {
						right = NUMBER;
					}
				}
			}


			if (opI == 0) {
				//printf("preParse: %d, isLabel: %d\n", preParse, isLabel);
				if (isLabel) {
					if (preParse) {
						if (labels.count(identifier) == 0) {
							labels.insert({ std::string(identifier), programI });
							if (verbose)printf("label %s\n", identifier.c_str());
							identifier.clear();
							//continue;
							goto spaceCheckBottom;
						}
						else {
							if (verbose)printf("label %s already exists\n", identifier.c_str());
							return 0;
						}
					}
					else {
						if (verbose)printf("cotinue\n");
						identifier.clear();
						continue;
					}
				}
				if (identifier.compare("#define") == 0 && !preParse 
					&& macroMap.count(identifier) == 0) {//then its a macro
					isMacro = true;
					//macroName = std::string(identifier) + '\0';
					//macroName = macroName.substr(1,macroName.size()-1);//remove the #
					identifier.clear();
					if (verbose)printf("macroDetected\n");
					opI = 1;
					continue;
				}
				//TODO: potential bug
				if (!preParse || true) {
					try {
						currentOp[0] = str_to_baseOp(identifier.c_str());
						if (currentOp[0] == opMap.at("nop")) {
							if (verbose)printf("actually a nop\n");
							if (!preParse)addToProgram(program, currentOp, programI);
							opI = 0;
							programI += getOpSize();
						}
						else {
							if (verbose)printf("added op %d\n", currentOp[0]);
							opI++;
						}
					}
					catch (const std::out_of_range& oor) {
						if (verbose)printf("unknown op %s\n", identifier.c_str());
						return 0;
					}
				}
				else {
					opI++;
				}

				//printf("'nop' vs '%s'\n", identifier.c_str());
				//lol
				if (false && strcmp(identifier.c_str(), "nop") == 0) {
					if (!preParse)addToProgram(program, currentOp, programI);
					programI += getOpSize();
					if (verbose)printf("tis but a nop\n");
					currentOp[0] = 0x69;
					opI = 0;
					//continue;
				}
			}
			else if (opI == 1) {
				//is in jmp family
				if(verbose)printf("currentOp[0]=%d\n", currentOp[0]);
				if (currentOp[0] >= 0x30 && currentOp[0] <= 0x36) {
					if (isInteger(identifier.c_str())) {
						int num = atoi(identifier.c_str());
						if (num > std::numeric_limits<cpu_t>::max()) {
							if (verbose)printf("number %d is too big, bigger than %d\n", num, (int)sizeof(short));
							return 0;
						}
						else {
							currentOp[opI] = num;
							if (currentOp[0] == 0x30 && left == REGISTER)
								currentOp[0] = 0x36;
							if (verbose)printf("numeber machine go %d brrrrrrrr\n", num);
							if (!preParse)addToProgram(program, currentOp, programI);
							programI += getOpSize();
							opI = 0;
						}
					}
				}
				else if (currentOp[0] == opMap.at("not")) {//NOT needs special attention
					currentOp[0] += static_cast<int>(left) - 1;//specific to Not instructions
					if (!preParse)addToProgram(program ,currentOp, programI);
					programI += getOpSize();
					opI = 0;
					if (verbose)printf("added the not opcode thing currenOp[0]: %d\n",currentOp[0]);
				}
				//gmov time lets goooo
				else if (currentOp[0] == opMap.at("gmov")) {
					currentOp[0] += static_cast<int>(left);//specific to gmov instructions
					if(!preParse)addToProgram(program, currentOp, programI);
					programI += getOpSize();
					opI = 0;
					if (verbose)printf("added gmov lets goo currenOp[0]: %d\n", currentOp[0]);
				}
				else if (preParse && isLabel) {
					programI += getOpSize();
					opI = 0;
				}
				else if (isMacro) {
					macroName = std::string(identifier);
					if (verbose)printf("macroName: '%s'\n",macroName.c_str());
					opI++;
				}
				else {
					opI++;
				}
			}
			else if (opI == 2) {
				if (isMacro) {
					macroBody = std::string(identifier);
					if (verbose)printf("macroBody: '%s'\n", macroBody.c_str());
					opI = 0;
					isMacro = false;
					macroMap.insert({std::string(macroName), std::string(macroBody)});
				}
				else {
					//if (verbose)printf("");
					short offset = opOffset(currentOp[0], static_cast<int>(left), static_cast<int>(right));
					if (offset == -1) {//offset invalid idk man
						if (verbose)printf("invalid offset\n");
						return 0;
					}
					else {
						if (verbose)printf("offset: %d, left: %d, right: %d\n", offset, left, right);
						currentOp[0] += offset;
						if(!preParse)addToProgram(program, currentOp, programI);

						programI += getOpSize();
						opI = 0;
					}
				}
			}
			if (verbose)printf("clear\n");
			identifier.clear();
					
			spaceCheckBottom:
			if(verbose)printf("i: %d, strLength-1: %d\n", i, (strLength - 1));
			if (preParse && i == (strLength - 1)) {
				if (verbose)printf("\n\nNORMAL PARSE\n");
				i = -1;
				programI = 0;
				opI = 0;
				preParse = false;
			}
			continue;
		}

		if (!isComment) {
			identifier += c;
			//printf("added %c\n",c);
		}
		//printf("i: %d, strLength-1: %d\n", i, (strLength - 1));
		if (preParse && i == (strLength - 1)) {
			actualParse:
			if (verbose)printf("\nACTUAL PARSE\n");
			i = -1;
			programI = 0;
			opI = 0;
			preParse = false;
			continue;
		}
	}
	return programI;
}

CPU_T
void CpuSimulator<cpu_t>::addToProgram(byte* program, cpu_t* op, int index) {
	if (verbose) {
		printf("added to program op ");
		bin(op[0]); printf(" "); bin(op[1]); printf(" ");  bin(op[2]);
		printf("\n");
	}

	char bytes = (getOpSize() - 1) / 2;
	if (verbose)printf("bytes: %d\n", bytes);

	program[index] = op[0];//this is probably already correct

	//gotta split the op[1] and op[2] into bytes
	for (int byte = 0; byte < bytes; byte++) {
		//this looks confusing and it kind of it
		//program[index + 1 + byte] = (((op[1] >> (byte*bits)) & maskByte)) >> byte * bits;
		auto a = op[1] >> (byte * 8);//get it into position for maskng
		auto b = a & maskByte;//mask only 1 byte
		auto c = b >> (byte * 8);//return it bac
		if (verbose) {
			printf("op[1] a: "); bin(a); printf(" b: "); bin(b); printf(" c: "); bin(c); printf("\n");
		}
		program[index + 1 + byte] = b;
	}

	for (int byte = 0; byte < bytes; byte++) {
		//program[index + (bits/8) + byte] = (((op[2] >> (byte * bits)) & maskByte)) >> byte * bits;
		auto a = op[2] >> (byte * 8);
		auto b = a & maskByte;
		auto c = b >> (byte * 8);
		if (verbose) {
			printf("op[2] a: "); bin(a); printf(" b: "); bin(b); printf(" c: "); bin(c); printf("\n");
		}
		program[index + bytes + byte + 1] = b;
	}
	if (verbose)printBitArray(program, getOpSize(), index);
}

extern "C"
inline void DONT_CALL_thisExistsToAvoidLinkingError() {
	CpuSimulator<short> bruh;
	CpuSimulator<int> bruh2;
	CpuSimulator<long> bruh3;
	CpuSimulator<long long> bruh4;

	//this might not have to exist
	cpu::getPrefix(NOTHING);
	cpu::fromPrefix("");

	cpu::str_to_args<short>("");
	cpu::arg_to_string<short>({ NOTHING, 0 });
	cpu::string_to_arg<short>("");

	cpu::str_to_args<int>("");
	cpu::arg_to_string<int>({ NOTHING, 0 });
	cpu::string_to_arg<int>("");

	cpu::str_to_args<long>("");
	cpu::arg_to_string<long>({ NOTHING, 0 });
	cpu::string_to_arg<long>("");

	cpu::str_to_args<long long>("");
	cpu::arg_to_string<long long>({ NOTHING, 0 });
	cpu::string_to_arg<long long>("");
}