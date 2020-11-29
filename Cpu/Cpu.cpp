// #pragma once

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
	std::map<MemoryType, MemoryTypeData> memoryTypeData;
	std::map<std::string, int> opMap;
	std::map<std::string, OpArgCombos> opArgs;
	std::map<std::string, std::string> macroMap;
	std::map<MemoryType, MemoryTypeRange<long>> memoryRanges;
	
	bool init = false;

	//cpu::MemoryType::MemoryType(int val, bool hasPrefix) {

	//}
	
	std::string getPrefix(MemoryType type) {
		switch (type) {
		case NUMBER:
			return "";
		case REGISTER:
			return "r";
		case MEMORY:
			return "mem";
			//return "m";
		case FLAG:
			return "f";
		case GRAPHICS:
			return "gp";
		case NOTHING:
			return "";
		case POINTER:
			return "ptr";
		}
	}

	//idk about NOTHING type but whatever breh
	cpu::MemoryType fromPrefix(std::string prefix) {
		switch (str2int(prefix.c_str())) {
		case str2int("r"):
			return REGISTER;
		case str2int("mem"):
		//case str2int("m"):
			return MEMORY;
		case str2int("f"):
			return FLAG;
		//case str2int("gp"):
		//	return GRAPHICS;
		case str2int("ptr"):
			return POINTER;
		default:
			return NUMBER;
		}
	}

	OpArgCombos::OpArgCombos(std::vector<MemoryType> left, std::vector<MemoryType> right) {
		this->left = left;
		this->right = right;
	}

	int str_to_baseOp(std::string str) {
		return opMap.at(removeSpaceAndLowerCase(str));//debug this
		//return opMap.at(removeSpace(toLower(str)));//debug this
	}

	int opOffset(short op, int leftType, int rightType) {
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
		else { //every other instruction
			if (leftType == static_cast<int>(MEMORY))offset += 0x04;
			else if (leftType == static_cast<int>(POINTER)) offset += 0x08;
			//else if (leftType == static_cast<int>(GRAPHICS)) offset += 0x08;

			offset += rightType;//this is ratty but it ees what eet ees
		}

		return offset;
	}

	MemoryTypeData::MemoryTypeData(bool hasSuffix) {
		this->hasSuffix = hasSuffix;
	}

	MemoryTypeData::MemoryTypeData() {
		this->hasSuffix = false;
	}
	
}