#pragma once

#include "Cpu2.h"

//using namespace cpu;

//idk if there is a better way to do this
#define NUMBER cpu::MemoryType::NUMBER
#define REGISTER cpu::MemoryType::REGISTER
#define MEMORY cpu::MemoryType::MEMORY
#define FLAG cpu::MemoryType::FLAG
#define GRAPHICS cpu::MemoryType::GRAPHICS
#define NOTHING cpu::MemoryType::NOTHING
#define POINTER cpu::MemoryType::POINTER

//std::map<std::string, int> opMap = std::map<std::string, int>();
//std::map<std::string, OpArgCombos> opArgs = std::map<std::string, OpArgCombos>();
//std::map<std::string, std::string> macroMap = std::map<std::string, std::string>();
//std::map<MemoryType, MemoryTypeRange<long>> memoryRanges = std::map<MemoryType, MemoryTypeRange<long>>();
namespace cpu {
	std::map<std::string, int> opMap;
	std::map<std::string, OpArgCombos> opArgs;
	std::map<std::string, std::string> macroMap;
	std::map<MemoryType, MemoryTypeRange<long>> memoryRanges;
	
	bool init = false;
	
	std::string cpu::getPrefix(MemoryType type) {
		switch (type) {
		case NUMBER:
			return "";
		case REGISTER:
			return "r";
		case MEMORY:
			return "m";
		case FLAG:
			return "f";
		case GRAPHICS:
			return "gp";
		case NOTHING:
			return "";
		}
	}

	//idk about NOTHING type but whatever breh
	cpu::MemoryType cpu::fromPrefix(std::string prefix) {
		switch (str2int(prefix.c_str())) {
		case str2int("r"):
			return REGISTER;
		case str2int("m"):
			return MEMORY;
		case str2int("f"):
			return FLAG;
		case str2int("gp"):
			return GRAPHICS;
		default:
			return NUMBER;
		}
	}

	cpu::OpArgCombos::OpArgCombos(std::vector<MemoryType> left, std::vector<MemoryType> right) {
		this->left = left;
		this->right = right;
	}

	int cpu::str_to_baseOp(std::string str) {
		return opMap.at(removeSpace(toLower(str)));//debug this
	}

	int cpu::opOffset(short op, int leftType, int rightType) {
		if (leftType == static_cast<int>(NUMBER))return -1;//invalid
		if (rightType == static_cast<int>(POINTER) && op > 0x0E) return -1;//only mov does right=gp
		//if (rightType == static_cast<int>(GRAPHICS) && op > 0x0E) return -1;//only mov does right=gp
		int offset = 0x00;

		if (op <= 0x0E) {//its a mov
			if (leftType == static_cast<int>(MEMORY))offset += 0x05;
			else if (leftType == static_cast<int>(POINTER)) offset += 0x0A;
			//else if (leftType == static_cast<int>(GRAPHICS)) offset += 0x0A;
			else if (leftType == static_cast<int>(FLAG)
				&& rightType == static_cast<int>(NUMBER)) offset = 0x0F;

			offset += rightType;
		}
		else {//every other instruction
			if (leftType == static_cast<int>(MEMORY))offset += 0x04;
			else if (leftType == static_cast<int>(POINTER)) offset += 0x08;
			//else if (leftType == static_cast<int>(GRAPHICS)) offset += 0x08;

			offset += rightType;//this is ratty but it ees what eet ees
		}

		return offset;
	}

	//void cpu::initMaps() {
	//	if (init)return;
	//	init = true;
	//
	//	//i now realize that i could have just made a typedef but i am too lazy to fix it and this works fine
	//	//naw i think this is better anyways
	//	auto toCombo = [](std::string op, std::vector<MemoryType> left, std::vector<MemoryType> right)
	//	{return std::pair<const std::string, OpArgCombos>{op, { left, right }}; };
	//
	//	opMap.insert({ "mov",0x00 });
	//	opArgs.insert(toCombo("mov", {REGISTER,MEMORY/*,GRAPHICS*/}, {NUMBER,REGISTER,MEMORY,FLAG/*,GRAPHICS*/ }));
	//
	//	opMap.insert({ "nop",0x37 });
	//	opArgs.insert(toCombo("nop", {  }, { }));
	//
	//	opMap.insert({ "add",0x10 });
	//	opArgs.insert(toCombo("add", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "sub",0x20 });
	//	opArgs.insert(toCombo("sub", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "jmp",0x30 });
	//	opArgs.insert(toCombo("jmp", { REGISTER,NUMBER}, { }));
	//
	//	opMap.insert({ "jgt",0x31 });
	//	opArgs.insert(toCombo("jgt", { NUMBER }, { }));
	//
	//	opMap.insert({ "jngt",0x32 });
	//	opArgs.insert(toCombo("jngt", { NUMBER }, { }));
	//
	//	opMap.insert({ "je",0x33 });
	//	opArgs.insert(toCombo("je", { NUMBER }, { }));
	//
	//	opMap.insert({ "jne",0x34 });
	//	opArgs.insert(toCombo("jne", { NUMBER }, { }));
	//
	//	opMap.insert({ "mul",0x40 });
	//	opArgs.insert(toCombo("mul", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "cmp",0x50 });
	//	opArgs.insert(toCombo("cmp", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "or",0x60 });
	//	opArgs.insert(toCombo("or", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "and",0x70 });
	//	opArgs.insert(toCombo("and", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "xor",0x80 });
	//	opArgs.insert(toCombo("xor", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "not",0x90 });
	//	opArgs.insert(toCombo("not", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "shl",0xA0 });
	//	opArgs.insert(toCombo("shl", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "shr",0xB0 });
	//	opArgs.insert(toCombo("shr", { REGISTER,MEMORY/*,GRAPHICS*/ }, { NUMBER,REGISTER,MEMORY,FLAG, }));
	//
	//	opMap.insert({ "gmov",0xC0 });
	//	opArgs.insert(toCombo("gmov", { REGISTER,MEMORY,/*GRAPHICS,*/FLAG,NUMBER }, {}));
	//}

	//template CpuSimulator<short>;
	//template CpuSimulator<int>;
	//template CpuSimulator<long>;
	//template CpuSimulator<long long>;
}