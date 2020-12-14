//#ifndef GENETIC_CPP
//#define GENETIC_CPP

//#include <vector>
//#include <string>
#include "Genetic.h"

using namespace cpu;
using namespace gene;
//using namespace std;//bruh ew get those stds out of here
//TODO: remove the stds because we want clean code only ok bois

#define NUMBER cpu::MemoryType::NUMBER
#define REGISTER cpu::MemoryType::REGISTER
#define MEMORY cpu::MemoryType::MEMORY
#define FLAG cpu::MemoryType::FLAG
#define GRAPHICS cpu::MemoryType::GRAPHICS
#define NOTHING cpu::MemoryType::NOTHING
//
namespace gene {
	bool compare(costInfo i, costInfo j) {
		return i.cost < j.cost;
	}

	/*
		min inclusive and max inclusive
	*/
	vector<vector<int>> generateX(vector<int> (*genFunction)(int x), int min, int max){
		vector<vector<int>> x;
		for(int i=min;i<=max;i++){
			x.push_back(genFunction(i));
		}
		return x;
	}

	vector<vector<int>> generateY(vector<int> (*genFunction)(vector<int> x), vector<vector<int>> inputs){
		vector<vector<int>> y;
		for(int i=0;i<inputs.size();i++){
			y.push_back(genFunction(inputs[i]));
		}
		return y;
	}

}