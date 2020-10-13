#pragma once
//#ifndef CPU_H
//#define CPU_H

#include <algorithm>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <ctype.h>

#include "helper.h"
//#include "../Evolutionary Assembly/testImp.cpp"

#ifdef NUMBER
#undef NUMBER
#endif // NUMBER
#ifdef REGISTER
#undef REGISTER
#endif // NUMBER
#ifdef MEMORY
#undef MEMORY
#endif // NUMBER
#ifdef FLAG
#undef FLAG
#endif // NUMBER
#ifdef GRAPHICS
#undef GRAPHICS
#endif // NUMBER
#ifdef NOTHING
#undef NOTHING
#endif // NUMBER

#define CPU_T template <class cpu_t>
//#define CPU_T template< class cpu_t = int, class = class std::enable_if<std::is_arithmetic<cpu_t>::value, cpu_t>::type>
//#define CPU_T template<typename cpu_t> \
//typename std::enable_if<std::is_arithmetic<cpu_t>::value>::type

#ifndef REGISTER_SIZE
#define REGISTER_SIZE 16
#endif

#ifndef MEMORY_SIZE
#define MEMORY_SIZE 0xFFF
#endif

#ifndef GRAPHICS_SIZE
#define GRAPHICS_SIZE 20
#endif

#define FLAGS 2

namespace cpu {
	typedef unsigned char byte;

	enum class MemoryType {
		NUMBER = 0,
		REGISTER = 1,
		MEMORY = 2,
		FLAG = 3,
		GRAPHICS = 4,
		NOTHING = 5
	};

#define NUMBER cpu::MemoryType::NUMBER
#define REGISTER cpu::MemoryType::REGISTER
#define MEMORY cpu::MemoryType::MEMORY
#define FLAG cpu::MemoryType::FLAG
#define GRAPHICS cpu::MemoryType::GRAPHICS
#define NOTHING cpu::MemoryType::NOTHING

	CPU_T
		struct OpArgument {
		MemoryType type;
		cpu_t val;
	};

	std::string getPrefix(MemoryType type);

	//idk about NOTHING type but whatever breh
	MemoryType fromPrefix(std::string prefix);

	CPU_T
		struct MemoryTypeRange {
		cpu_t min;
		cpu_t max;
	};

	struct OpArgCombos {
		std::vector<MemoryType> left;
		std::vector<MemoryType> right;
		OpArgCombos(std::vector<MemoryType> left, std::vector<MemoryType> right);
	};

	extern std::map<std::string, int> opMap;
	extern std::map<std::string, OpArgCombos> opArgs;
	extern std::map<std::string, std::string> macroMap;
	extern std::map<MemoryType, MemoryTypeRange<long>> memoryRanges;

	//TODO: add range checks for the different memory types
	CPU_T
	inline OpArgument<cpu_t> string_to_arg(std::string arg) {
		if (arg.length() <= 0) return { NOTHING,0 };
		if (opMap.count(toLower(arg))) return { NOTHING,0 };//throw error cus its not supposed to be an op

		std::string prefix;
		cpu_t value;
		int i = 0;
		for (; i < arg.size(); i++) {
			if (!isalpha(arg[i]))
				break;
			else
				prefix.push_back(arg[i]);
		}
		//printf("prefixOriginal: %s\n", prefix.c_str());
		MemoryType type = fromPrefix(prefix);

		std::string valueZ;
		for (; i < arg.size(); i++) {
			valueZ.push_back(arg[i]);
		}

		if (isInteger(valueZ)) {
			//printf("type: %d, val: %d\n", type, stol(valueZ));
			return { type, (cpu_t)stol(valueZ) };
		}
		else {
			//TODO: throw error here
			return { NOTHING, 0 };
		}
		return { NOTHING,0 };
	}

	//this accepts an entire opcode the other method accepts only single args
	CPU_T
	inline std::pair<OpArgument<cpu_t>, OpArgument<cpu_t>> str_to_args(std::string str) {
		std::vector<std::string> parts = splitString(&str, ' ');
		//std::tuple<std::string, OpArgument, OpArgument> parsedParts;
		switch (parts.size()) {
		case 1:// e.g 'nop'
			return { { NOTHING,0 }, { NOTHING,0 } };
		case 2:// e.g 'jmp 5'
			return { string_to_arg<cpu_t>(parts[1]), { NOTHING,0 } };
		case 3://e.g 'mov r0 1'
			return { string_to_arg<cpu_t>(parts[1]), string_to_arg<cpu_t>(parts[2]) };
		default:
			//ideally throw error here
			return { { NOTHING,0 }, { NOTHING,0 } };
		}
	}

	CPU_T
	inline std::string arg_to_string(OpArgument<cpu_t> arg) {
		if (arg.type == NOTHING) return std::string("");
		std::string prefix = getPrefix(arg.type);
		std::string suffix = std::to_string(arg.val);
		return prefix + suffix;
	}

	int str_to_baseOp(std::string str);

	int opOffset(short op, int leftType, int rightType);

	extern bool init;
	//bool init = false;

	inline std::pair<const std::string, OpArgCombos> inline toCombo(std::string op, std::vector<MemoryType> left, std::vector<MemoryType> right) {
		return std::pair<const std::string, OpArgCombos>{op, { left, right }};
	}

	inline void initMaps() {
		//initMaps_IMP();
		if (init)return;
		init = true;

		//i now realize that i could have just made a typedef but i am too lazy to fix it and this works fine
		//naw i think this is better anyways
		//auto toCombo = [](std::string op, std::vector<MemoryType> left, std::vector<MemoryType> right)
		//{return std::pair<const std::string, OpArgCombos>{op, { left, right }}; };

		opMap.insert({ "mov",0x00 });
		opArgs.insert(toCombo("mov", { REGISTER, MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG/*,GRAPHICS*/ }));

		opMap.insert({ "nop",0x37 });
		opArgs.insert(toCombo("nop", {  }, { }));

		opMap.insert({ "add",0x10 });
		opArgs.insert(toCombo("add", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "sub",0x20 });
		opArgs.insert(toCombo("sub", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "jmp",0x30 });
		opArgs.insert(toCombo("jmp", { REGISTER,NUMBER }, { }));

		opMap.insert({ "jgt",0x31 });
		opArgs.insert(toCombo("jgt", { NUMBER }, { }));

		opMap.insert({ "jngt",0x32 });
		opArgs.insert(toCombo("jngt", { NUMBER }, { }));

		opMap.insert({ "je",0x33 });
		opArgs.insert(toCombo("je", { NUMBER }, { }));

		opMap.insert({ "jne",0x34 });
		opArgs.insert(toCombo("jne", { NUMBER }, { }));

		opMap.insert({ "mul",0x40 });
		opArgs.insert(toCombo("mul", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "cmp",0x50 });
		opArgs.insert(toCombo("cmp", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "or",0x60 });
		opArgs.insert(toCombo("or", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "and",0x70 });
		opArgs.insert(toCombo("and", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "xor",0x80 });
		opArgs.insert(toCombo("xor", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "not",0x90 });
		opArgs.insert(toCombo("not", { REGISTER,MEMORY/*,GRAPHICS*/ }, { /*NUMBER,REGISTER,MEMORY,FLAG,*/ }));

		opMap.insert({ "shl",0xA0 });
		opArgs.insert(toCombo("shl", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "shr",0xB0 });
		opArgs.insert(toCombo("shr", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		//opMap.insert({ "gmov",0xC0 });
		//opArgs.insert(toCombo("gmov", { REGISTER,MEMORY,/*GRAPHICS,*/FLAG,NUMBER }, {}));
	}

	CPU_T
		class CpuSimulator {
		private:
			cpu_t registers[REGISTER_SIZE];//16 i guess idk
			cpu_t flags[FLAGS];//0 = GT, 1 = E
			cpu_t memory[MEMORY_SIZE];//the memory O.o
			//TODO: add stack pointers and instructions
			cpu_t ip = 0;//instruction pointer

			//TODO: make the size equal to the amount of pixels on screen mah boi
			//TODO: implement the actual ppu so we know whats up lol
			cpu_t graphics[GRAPHICS_SIZE];//the graphics memory
			cpu_t gp = 0;//graphics pointer

			std::atomic<bool> interruptz{ false };

			void initMaps() {
				typedef std::pair<MemoryType, MemoryTypeRange<cpu_t>> rangePair;
				typedef std::pair<std::tuple<std::string, MemoryType>, MemoryTypeRange<cpu_t> > bruhRange;
				typedef std::pair<std::tuple<std::string, MemoryType, MemoryType>, MemoryTypeRange<cpu_t> > bruhRange2;

				//cpu_t number = std::numeric_limits<cpu_t>::max() - 1;
				cpu_t number = 100;
				printf("number: %d\n", number);

				rangesMap.insert(rangePair{ NUMBER, {-number, number} });
				rangesMap.insert(rangePair{ REGISTER, {0,REGISTER_SIZE - 1} });
				rangesMap.insert(rangePair{ MEMORY, {0,MEMORY_SIZE - 1} });
				rangesMap.insert(rangePair{ FLAG, {0,FLAGS - 1} });
				rangesMap.insert(rangePair{ GRAPHICS, {0,GRAPHICS_SIZE - 1} });//TODO i havent implemented yet

				bruhMap.insert(bruhRange{ {"jmp", NUMBER}, {0, number} });
				bruhMap.insert(bruhRange{ {"jgt", NUMBER}, {0, number} });
				bruhMap.insert(bruhRange{ {"jngt", NUMBER}, {0, number} });
				bruhMap.insert(bruhRange{ {"je", NUMBER}, {0, number} });
				bruhMap.insert(bruhRange{ {"jne", NUMBER}, {0, number} });

				//mov gp 0-number
				bruhMap2.insert(bruhRange2{ {"mov", GRAPHICS, NUMBER}, {0, number} });
			}
		public:
			bool verbose = false;
			bool step = false;
			std::map<MemoryType, MemoryTypeRange<cpu_t>> rangesMap;

			std::map<std::tuple<std::string, MemoryType>, MemoryTypeRange<cpu_t> > bruhMap;
			std::map<std::tuple<std::string, MemoryType, MemoryType>, MemoryTypeRange<cpu_t> > bruhMap2;

			CpuSimulator() {
				cpu::initMaps();
				initMaps();
			}

			void interrupt() {
				this->interruptz.store(true);
			}

			void dumpRegisters() {
				for (cpu_t i = 0; i < REGISTER_SIZE; i++) {
					printf("r%d: %d\n", i, registers[i]);
				}
			}

			void resetRegisters() {
				for (cpu_t i = 0; i < REGISTER_SIZE; i++) {
					registers[i] = 0;
				}
			}

			void resetMemory() {
				for (cpu_t i = 0; i < MEMORY_SIZE; i++) {
					memory[i] = 0;
				}
			}

			cpu_t getRegister(unsigned char r) {
				//TODO: fix this one and getMemory I just cant figure out how to throw excpeptions lol
				//if (r > 15 || r < 0) throw out_of_range("");
				return registers[r];
			}

			cpu_t getMemory(unsigned short m) {
				//if (m > (0xFFFF - 1) || m < 0) throw out_of_range();
				return memory[m];
			}

			void dumpFlags() {
				for (cpu_t i = 0; i < FLAGS; i++) {
					printf("f%d: %d\n", i, flags[i]);
				}
			}

			void dumpPointers() {
				printf("ip: %d\n", ip);
				printf("gp: %d\n", gp);
			}

			bool setRegister(unsigned char r, size_t value) {
				if (r > REGISTER_SIZE - 1 || r < 0) return false;//bound check
				registers[r] = value;
				return true;
			}

			bool setMemory(unsigned short m, size_t value) {
				if (m > (MEMORY_SIZE - 1) || m < 0) return false;//bound check
				memory[m] = value;
				return true;
			}

			void resetFlags() {
				for (cpu_t i = 0; i < FLAGS; i++) {
					flags[i] = 0;
				}
			}

			char getOpSize() {
				return 2 * sizeof(cpu_t) + 1;
			}

			void runProgram(byte* code, int progLength) {
				//dumpRegisters();
				resetRegisters();
				resetFlags();
				if (verbose) {
					printf("test\n");
					printf("code[0] = %d\n", code[0]);
					printf("opSize = %d\n", getOpSize());
				}
				for (ip = 0; ip < progLength; ip += getOpSize()) {
					if (interruptz.load()) {
						this->interruptz.store(false);
						//printf("interrupt\n");
						break;
					}
					std::vector<byte> op(getOpSize(), 0);
					for (int j = 0; j < getOpSize(); j++) {
						op[j] = code[ip + j];
					}

					//if (verbose)printBitArray(code, getOpSize(), 0);
					executeOp(op);
				}
			}

			bool executeOp(cpu_t op, cpu_t left, cpu_t right) {
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
					//BUG: right is -1434654139
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
			cpu_t jmp_range_check(cpu_t left) {
				return max(0, getOpSize() * ((int)((float)left / (float)getOpSize())));
			}

			bool executeOp(std::vector<byte> code) {
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

			int assemble(byte* prog, std::string path) {
				if (verbose)printf("trying open bruh\n");
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
				str.append(" ");//"do this in rememberance of me" -Big Space
				if (verbose)printf("closing file\n\n");
				return assemble(prog, str.c_str(), str.size());
			}

			int assemble(byte* program, const char str[], int strLengthA) {
				const int strLength = strLengthA + 1;
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
				//bool applyingMacro = false;//to be implemented STINKY NOT
				std::string macroName;
				std::string macroBody;
				int macroIndex = 0;

				if (verbose)printf("PREPARSE\n");
				for (int i = 0; i < strLength; i++) {
					const char c = (i == strLength-1) ? ' ' : str[i];//TODO: added this
					if (c == ';')isComment = true;
					//isComment = (c == ';');
					//printf("'%c%s' ", c, isComment ? "(com)" : "");
					if (c == '\n' || c == EOF) {
						isComment = false;
						if (isMacro && false) {
							isMacro = false;
							macroMap.insert({ std::string(macroName) + '\0',
								std::string(macroBody) + '\0' });
							if (verbose)printf("macroBody: '%s'\n", macroBody.c_str());
						}
					}
					else if (isMacro && false) {
						macroBody += c;
						continue;
					}

					if (isspace(c) || c == '\0' || c == EOF) {//then make new thing or something idk
						if ((c == EOF /* || strLength - 1 == i */) && preParse) {
							printf("going to phase 2\n");
							goto actualParse;
						}
						else if (
							(!(preParse && i == (strLength - 1))
								&& (removeSpace(identifier).empty()
									|| identifier.empty())) || isComment)
							continue;
						//identifier += '\0';

						//char *lastChar = identifier.back();
						//printf("identifier.end() : %c\n", lastChar);

						if (verbose)printf("identifier size: %d\n", (int)strlen(identifier.c_str()));
						if (strlen(identifier.c_str()) > 0) {
							isLabel = identifier[strlen(identifier.c_str()) - 1] == ':';
						}
						else {
							//printf("mega bad?");
							goto actualParse;
						}

						if (macroMap.count(identifier) == 1) {
							//do the thing where you
							if (verbose)printf("applied macro '%s', ", identifier.c_str());
							identifier = std::string(macroMap.at(identifier));
							if (verbose)printf("macroBody: '%s'\n", identifier.c_str());
						}

						if (verbose)printf("identifier: '%s', opI: %d, programI: %d\n", identifier.c_str(), opI, programI);

						if (isLabel && opI != 0) {
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
							else if (!preParse) {
								if (verbose)printf("label %s doesnt exist\n", identifier.c_str());
								return 0;
							}
						}
						else if (identifier[0] == 'r' && opI != 0) {//tis but a register
							currentOp[opI] = atoi(identifier.substr(1, identifier.size() - 1).c_str());//ecks dee
							if (verbose)printf("%s -> register %d\n", identifier.c_str(), currentOp[opI]);
							if (opI == 1) {
								left = REGISTER;
							}
							else if (opI == 2) {
								right = REGISTER;
							}

						}
						else if (identifier[0] == 'f' && opI != 0) {//tis but a flag
							currentOp[opI] = atoi(identifier.substr(1, identifier.size() - 1).c_str());//ecks dee
							if (verbose)printf("%s -> flag %d\n", identifier.c_str(), currentOp[opI]);
							if (opI == 1) {
								left = FLAG;
							}
							else if (opI == 2) {
								right = FLAG;
							}
						}
						else if (identifier[0] == 'm' && opI != 0) {//tis but a memory locationN
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
						else if (identifier == "gp" && opI != 0) {
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
							if (abs(num) > std::numeric_limits<cpu_t>::max()-1) {
								if (verbose)printf("number %d is too big, bigger than %d\n", num, sizeof(cpu_t));
								return 0;
							}
							else {
								currentOp[opI] = num;
								if (verbose)printf("first parse currentOp[%d] = %d\n", opI, num);
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
							if (verbose)printf("currentOp[0]=%d\n", currentOp[0]);
							if (currentOp[0] >= 0x30 && currentOp[0] <= 0x36) {
								if (/*isInteger(identifier.c_str())*/
									left == NUMBER || (currentOp[0] == 0x30 && left == REGISTER)) {
									int num = atoi(identifier.c_str());
									if (abs(num) > std::numeric_limits<cpu_t>::max()-1) {
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
								if (!preParse)addToProgram(program, currentOp, programI);
								programI += getOpSize();
								opI = 0;
								if (verbose)printf("added the not opcode thing currenOp[0]: %d\n", currentOp[0]);
							}
							//gmov time lets goooo
							else if (currentOp[0] == opMap.at("gmov")) {
								currentOp[0] += static_cast<int>(left);//specific to gmov instructions
								if (!preParse)addToProgram(program, currentOp, programI);
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
								if (verbose)printf("macroName: '%s'\n", macroName.c_str());
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
								macroMap.insert({ std::string(macroName), std::string(macroBody) });
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
									if (!preParse)addToProgram(program, currentOp, programI);

									programI += getOpSize();
									opI = 0;
								}
							}
						}
						if (verbose)printf("clear\n");
						identifier.clear();

					spaceCheckBottom:
						if (verbose)printf("i: %d, strLength-1: %d\n", i, (strLength - 1));
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
						printf("arrived normall");
					actualParse:
						if (verbose)printf("\nACTUAL PARSE\n");
						i = -1;
						programI = 0;
						opI = 0;
						preParse = false;
						identifier.clear();
						continue;
					}
				}
				return programI;
			}

			const static int maskByte = 0b11111111;

			void addToProgram(byte* program, cpu_t* op, int index) {
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

	};

	inline void initMaps_IMP() {
		if (init)return;
		init = true;

		//i now realize that i could have just made a typedef but i am too lazy to fix it and this works fine
		//naw i think this is better anyways
		auto toCombo = [](std::string op, std::vector<MemoryType> left, std::vector<MemoryType> right)
		{return std::pair<const std::string, OpArgCombos>{op, { left, right }}; };

		typedef int bruh;

		opMap.insert({ "mov",0x00 });
		opArgs.insert(toCombo("mov", { REGISTER, MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG/*,GRAPHICS*/ }));

		opMap.insert({ "nop",0x37 });
		opArgs.insert(toCombo("nop", {  }, { }));

		opMap.insert({ "add",0x10 });
		opArgs.insert(toCombo("add", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "sub",0x20 });
		opArgs.insert(toCombo("sub", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "jmp",0x30 });
		opArgs.insert(toCombo("jmp", { REGISTER,NUMBER }, { }));

		opMap.insert({ "jgt",0x31 });
		opArgs.insert(toCombo("jgt", { NUMBER }, { }));

		opMap.insert({ "jngt",0x32 });
		opArgs.insert(toCombo("jngt", { NUMBER }, { }));

		opMap.insert({ "je",0x33 });
		opArgs.insert(toCombo("je", { NUMBER }, { }));

		opMap.insert({ "jne",0x34 });
		opArgs.insert(toCombo("jne", { NUMBER }, { }));

		opMap.insert({ "mul",0x40 });
		opArgs.insert(toCombo("mul", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "cmp",0x50 });
		opArgs.insert(toCombo("cmp", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "or",0x60 });
		opArgs.insert(toCombo("or", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "and",0x70 });
		opArgs.insert(toCombo("and", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "xor",0x80 });
		opArgs.insert(toCombo("xor", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "not",0x90 });
		opArgs.insert(toCombo("not", { REGISTER,MEMORY/*,GRAPHICS*/ }, { /*NUMBER,REGISTER,MEMORY,FLAG,*/ }));

		opMap.insert({ "shl",0xA0 });
		opArgs.insert(toCombo("shl", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		opMap.insert({ "shr",0xB0 });
		opArgs.insert(toCombo("shr", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));

		//opMap.insert({ "gmov",0xC0 });
		//opArgs.insert(toCombo("gmov", { REGISTER,MEMORY,/*GRAPHICS,*/FLAG,NUMBER }, {}));
	}

}



//TODO: add range checks for the different memory types



//#endif