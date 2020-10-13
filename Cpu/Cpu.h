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

	enum class MemoryType{
	NUMBER = 0,
	REGISTER = 1,
	MEMORY = 2,
	FLAG = 3,
	GRAPHICS = 4,
	NOTHING = 5
	};

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

	//extern std::map<std::string, int> opMap;
	//extern std::map<std::string, OpArgCombos> opArgs;
	//extern std::map<std::string, std::string> macroMap;
	//extern std::map<MemoryType, MemoryTypeRange<long>> memoryRanges;

	//this accepts an entire opcode the other method accepts only single args
	CPU_T
	std::pair<OpArgument<cpu_t>, OpArgument<cpu_t>> str_to_args(std::string str);

	CPU_T
	std::string arg_to_string(OpArgument<cpu_t> arg);

	//TODO: add range checks for the different memory types
	CPU_T
	OpArgument<cpu_t> string_to_arg(std::string arg);

	int str_to_baseOp(std::string str);

	int opOffset(short op, int leftType, int rightType);

	extern bool init;
	//bool init = false;

	void initMaps();

	//https://stackoverflow.com/questions/14294267/class-template-for-numeric-types
	//ill learn this stuff eventually bruv dont worry atleast im giving credit
	CPU_T
		class CpuSimulator {//this thing be 32 bit i guess
		private:
			cpu_t registers[REGISTER_SIZE];//16 i guess idk
			cpu_t flags[FLAGS];//0 = GT, 1 = E
			cpu_t memory[MEMORY_SIZE];//the memory O.o
			//TODO: add stack pointers and instructions
			cpu_t ip = 0;//instruction pointer

			//TODO: make the size equal to the amount of pixels on screen mah boi
			cpu_t graphics[GRAPHICS_SIZE];//the graphics memory
			cpu_t gp = 0;//graphics pointer

			void initMaps();
		public:
			bool verbose = false;
			bool step = false;
			std::map<MemoryType, MemoryTypeRange<cpu_t>> rangesMap;

			std::map<std::tuple<std::string, MemoryType>, MemoryTypeRange<cpu_t> > bruhMap;
			std::map<std::tuple<std::string, MemoryType, MemoryType>, MemoryTypeRange<cpu_t> > bruhMap2;

			CpuSimulator();

			void dumpRegisters();

			void resetRegisters();

			void resetMemory();

			cpu_t getRegister(unsigned char r);

			cpu_t getMemory(unsigned short m);

			void dumpFlags();

			void dumpPointers();

			bool setRegister(unsigned char r, size_t value);

			bool setMemory(unsigned short m, size_t value);

			void resetFlags();

			char getOpSize();

			void runProgram(byte* code, int progLength);

			bool executeOp(cpu_t op, cpu_t left, cpu_t right);

			//always round down
			//and make sure its greater than zero
			//ill look back at this and not understand a single word of it
			cpu_t jmp_range_check(cpu_t left);

			bool executeOp(std::vector<byte> code);

			int assemble(byte* prog, std::string path);

			int assemble(byte* program, const char str[], int strLength);

			const static int maskByte = 0b11111111;

			void addToProgram(byte* program, cpu_t* op, int index);
	};
}

//#include "Cpu.cpp"
#include "CpuTemplates.cpp"

//#endif