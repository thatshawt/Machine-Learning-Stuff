#include <algorithm>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>

using byte = unsigned char;
namespace cpu {
	static std::string toLower(std::string str) {
		std::string result = std::string(str);
		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return result;
	}

	static std::map<std::string, int> opMap;
	static std::map<std::string, std::string> macroMap;

	static int str_to_baseOp(std::string str) {
		return opMap.at(toLower(str));
	}

	enum MemoryType {
		NUMBER = 0,
		REGISTER = 1,
		MEMORY = 2,
		FLAG = 3,
		GRAPHICS = 4
	};

	static int opOffset(short op, int leftType, int rightType) {
		if (leftType == NUMBER)return -1;//invalid
		if (rightType == GRAPHICS && op > 0x0E) return -1;//only mov does right=gp
		int offset = 0x00;
		
		if (op <= 0x0E) {//its a mov
			if (leftType == MEMORY)offset += 0x05;
			else if (leftType == GRAPHICS) offset += 0x0A;

			offset += rightType;
		}
		else {//every other instruction
			if (leftType == MEMORY)offset += 0x04;
			else if (leftType == GRAPHICS) offset += 0x08;
		
			offset += rightType;//this is ratty but it ees what eet ees
		}

		return offset;
	}

	static void initMaps() {
		opMap.insert({ "mov",0x00 });

		opMap.insert({ "nop",0x37 });

		opMap.insert({ "add",0x10 });
		opMap.insert({ "sub",0x20 });

		opMap.insert({ "jmp",0x30 });
		opMap.insert({ "jgt",0x31 });
		opMap.insert({ "jngt",0x32 });
		opMap.insert({ "je",0x33 });
		opMap.insert({ "jne",0x34 });

		opMap.insert({ "mul",0x40 });

		opMap.insert({ "cmp",0x50 });

		opMap.insert({ "or",0x60 });
		opMap.insert({ "and",0x70 });
		opMap.insert({ "xor",0x80 });
		opMap.insert({ "not",0x90 });


		opMap.insert({ "shl",0xA0 });
		opMap.insert({ "shr",0xB0 });

		opMap.insert({ "gmov",0xC0 });



		//macroMap.insert({"",""});
	}

	class Op {
	private:

	public:
		virtual int toOp(MemoryType left, MemoryType right) = 0;
	};

	template <class size_type>
	class CpuSimulator {//this thing be 32 bit i guess
	private:
		size_type registers[16];//16 i guess idk
		size_type flags[2];//0 = GT, 1 = E
		size_type memory[0xFFFF];//the memory O.o
		size_type ip = 0;//instruction pointer

		//TODO: make the size equal to the amount of pixels on screen mah boi
		size_type graphics[0xFFFF];//the graphics memory
		size_type gp = 0;//graphics pointer
	public:
		bool verbose = false;
		bool step = false;
		void dumpRegisters() {
			for (size_type i = 0; i < 16; i++) {
				printf("r%d: %d\n", i, registers[i]);
			}
		}

		void resetRegisters() {
			for (size_type i = 0; i < 16; i++) {
				registers[i] = 0;
			}
		}

		void dumpFlags() {
			for (size_type i = 0; i < 2; i++) {
				printf("f%d: %d\n", i, flags[i]);
			}
		}

		void dumpPointers() {
			printf("ip: %d\n", ip);
			printf("gp: %d\n", gp);
		}

		bool setRegister(char r, size_t value) {
			if (r > 15 || r < 0) return false;//bound check
			registers[r] = value;
			return true;
		}

		bool setMemory(short m, size_t value) {
			if (m > (0xFFFF-1) || m < 0) return false;//bound check
			memory[m] = value;
			return true;
		}

		void resetFlags() {
			for (size_type i = 0; i < 2; i++) {
				flags[i] = 0;
			}
		}

		char getOpSize() {
			return 2 * sizeof(size_type) + 1;
		}

		void runProgram(byte code[], int progLength) {
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

		bool executeOp(std::vector<byte> code) {
			if (step)getchar();
			size_type op = code[0];
			size_type left;
			size_type right;

			char bytes = (getOpSize() - 1) / 2;

			for (int byte = bytes; byte > 0; byte--) {
				left = (left << 8) | code[byte];
			}
			for (int byte = bytes; byte > 0; byte--) {
				//if (verbose) 
				//	printf("added %d: ", bytes + byte); bin(code[bytes + byte]);
				
				right = (right << 8) | code[bytes + byte];
			}

			if (verbose) {
				printf("executed op:%d, left: %d, right: %d\n", op, left, right);
				printf("ip: %d\n",ip);
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
				ip = left - getOpSize();
				if (verbose)printf("jmp to %d\n", left);
				break;
			case 0x31://jumpIfGT location
				ip = (flags[0] * (left - getOpSize())) + (!flags[0] * ip);
				break;
			case 0x32://jumpIfNotGT location
				ip = (!flags[0] * (left - getOpSize())) + (flags[0] * ip);
				break;
			case 0x33://jumpIfEqual location
				ip = (flags[1] * (left - getOpSize())) + (!flags[1] * ip);
				break;
			case 0x34://jumpIfNotEqual location
				ip = (!flags[1] * (left - getOpSize())) + (flags[1] * ip);
				break;
			case 0x35://jumpIfNotZero location
				ip = (!flags[1] * (left - getOpSize())) + (flags[1] * ip);
				break;
			case 0x36://jump register
				ip = registers[left] - getOpSize();
				break;
			case 0x37://the epic noop
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

		int toNum(std::string str) {
			std::string result;
			for (int i = 0; i < str.size(); i++) {
				char c = str[i];
				if (isdigit(c));
			}
			return 1;
		}

		static std::string removeSpace(std::string str) {
			std::string result;
			for (int i = 0; i < str.size(); i++) {
				if (!isspace(str[i]))result += str[i];
			}
			return result;
		}

		int assemble(byte* prog, std::string path) {
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

		int assemble(byte* program, const char str[], int strLength) {
			std::map<std::string, int> labels;
			size_type currentOp[3];//ops can only be 3 max lolll
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

					printf("identifier size: %d\n", strlen(identifier.c_str()));
					if (strlen(identifier.c_str()) > 0){
						isLabel = identifier[strlen(identifier.c_str()) - 1] == ':';
					} else {
						goto actualParse;
					}

					if (macroMap.count(identifier) == 1) {
						//do the thing where you 
						if (verbose)printf("applied macro '%s', ", identifier.c_str());
						identifier = std::string(macroMap.at(identifier));
						if (verbose)printf("macroBody: '%d'\n", identifier.c_str());
					}

					if (verbose)printf("identifier: '%s', opI: %d, programI: %d\n", identifier.c_str(), opI, programI);

					if ( isLabel && opI != 0) {
						if (labels.count(identifier) == 1) {
							//currentOp[opI] = labels.at(identifier);
							identifier = std::to_string(labels.at(identifier));
							if (verbose)printf("replaced label with %s\n", identifier.c_str());
							currentOp[opI] = atoi(identifier.c_str());
							if (opI == 1) {
								left = MemoryType::NUMBER;
							}
							else if (opI == 2) {
								right = MemoryType::NUMBER;
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
							left = MemoryType::REGISTER;
						}
						else if (opI == 2) {
							right = MemoryType::REGISTER;
						}

					}
					else if ( identifier[0] == 'f' && opI != 0) {//tis but a flag
						currentOp[opI] = atoi(identifier.substr(1, identifier.size() - 1).c_str());//ecks dee
						if (verbose)printf("%s -> flag %d\n", identifier.c_str(), currentOp[opI]);
						if (opI == 1) {
							left = MemoryType::FLAG;
						}
						else if (opI == 2) {
							right = MemoryType::FLAG;
						}
					}
					else if ( identifier[0] == 'm' && opI != 0) {//tis but a memory locationN
						currentOp[opI] = atoi(identifier.substr(1, identifier.size() - 1).c_str());//ecks dee
						if (verbose)printf("%s -> memory %d\n", identifier.c_str(), currentOp[opI]);
						if (opI == 1) {
							left = MemoryType::MEMORY;
						}
						else if (opI == 2) {
							right = MemoryType::MEMORY;
						}
					}
					//tis but the graphics pointer
					else if ( identifier == "gp" && opI != 0) {
						if (verbose)printf("graphics pointer\n");
						if (opI == 1) {
							left = MemoryType::GRAPHICS;
						}
						else if (opI == 2) {
							right = MemoryType::GRAPHICS;
						}
					}
					//!preParse && 
					else if (isInteger(identifier.c_str())) {
						int num = stoi(identifier);
						if (num > pow(2, (8 * sizeof(size_type) - 1))) {
							if (verbose)printf("number %d is too big, bigger than %d\n", num, sizeof(size_type));
							return 0;
						}
						else {
							currentOp[opI] = num;
							if (verbose)printf("currentOp[%d] = %d\n", opI, num);
							if (opI == 1) {
								left = MemoryType::NUMBER;
							}
							else if (opI == 2) {
								right = MemoryType::NUMBER;
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
						if (!preParse) {
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
						if (currentOp[0] >= 0x30 && currentOp[0] <= 0x36) {
							if (isInteger(identifier.c_str())) {
								int num = atoi(identifier.c_str());
								if (num > pow(2, (8 * sizeof(short) - 1))) {
									if (verbose)printf("number %d is too big, bigger than %d\n", num, sizeof(short));
									return 0;
								}
								else {
									currentOp[opI] = num;
									if (currentOp[0] == 0x30 && left == MemoryType::REGISTER)
										currentOp[0] = 0x36;
									if (verbose)printf("numeber machine go %d brrrrrrrr\n", num);
									if (!preParse)addToProgram(program, currentOp, programI);
									programI += getOpSize();
									opI = 0;
								}
							}
						}
						else if (currentOp[0] == opMap.at("not")) {//NOT needs special attention
							currentOp[0] += left - 1;//specific to Not instructions
							if (!preParse)addToProgram(program ,currentOp, programI);
							programI += getOpSize();
							opI = 0;
							if (verbose)printf("added the not opcode thing currenOp[0]: %d\n",currentOp[0]);
						}
						//gmov time lets goooo
						else if (currentOp[0] == opMap.at("gmov")) {
							currentOp[0] += left;//specific to gmov instructions
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
							short offset = opOffset(currentOp[0], left, right);
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
					printf("i: %d, strLength-1: %d\n", i, (strLength - 1));
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
				printf("i: %d, strLength-1: %d\n", i, (strLength - 1));
				if (preParse && i == (strLength - 1)) {
					actualParse:
					if (verbose)printf("\n\ACTUAL PARSE\n");
					i = -1;
					programI = 0;
					opI = 0;
					preParse = false;
					continue;
				}
			}
			return programI;
		}

		static void bin(unsigned n)
		{
			//thx stackoverflow
			/* step 1 */
			if (n > 1)
				bin(n / 2);

			/* step 2 */
			printf("%d", n % 2);
		}

		static void printBitArray(byte* bytes, int length, int offset) {
			for (int i = 0; i < length; i++) {
				bin(bytes[i]); printf(" ");
			}
			printf("\n");
		}

		const static int maskByte = 0b11111111;

		void addToProgram(byte* program, size_type* op, int index) {
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

		static inline bool isInteger(const std::string& s)
		{
			if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

			char* p;
			strtol(s.c_str(), &p, 10);

			return (*p == 0);
		}

	};
}