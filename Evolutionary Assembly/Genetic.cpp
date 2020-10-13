#pragma once
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
	bool gene::compare(costInfo i, costInfo j) {
		return i.cost < j.cost;
	}
//
//	DNA_T
//	void gene::GeneticAlgorithm<dna_t>::train(int generations, int maxSpecies, vector<vector<int>> x, vector<vector<int>> y) {
//		vector<dna_t> dna;
//
//		for (int species = 0; species < maxSpecies; species++) {
//			dna.push_back(getRandomDna());
//		}
//
//		for (int gen = 0; gen < generations; gen++) {
//			printf("Generation %d\n", gen);
//			vector<costInfo> costs(maxSpecies, { 0,0 });
//			for (int species = 0; species < maxSpecies; species++) {
//				printf("Species_Start: %d\n", species);
//				//printf("Code:%s\n\n", combine(dna[species]).c_str());
//				double cost = 0;
//				for (int i = 0; i < x.size(); i++) {
//					printf("Apply input: %d, Input: %d\n", i, x[i]);
//					applyInput(x[i]);
//					printf("applied input\n");
//					applyDna(dna[species]);
//					printf("applied dna\n");
//
//					//printf("i: %d\n", i);
//					auto expected = y[i];
//					printf("got expected\n");
//					auto result = getResult(x[i]);
//					printf("got result\n");
//
//					this->cost(&cost, expected, result);
//					printf("calculated cost\n");
//				}
//				postIterateCost(&cost, x);
//				costs[species].species = species;
//				costs[species].cost = cost;
//				//totalCost += cost;
//				printf("Species_End: %d cost: %f\n\n", species, cost);
//
//				if (cost <= 0.01) {
//					printf("you win baws\n");
//					printf("%s\n", combine((vector<string>)bestDna).c_str());
//					return;
//				}
//			}
//			std::sort(costs.begin(), costs.end(), compare);
//
//			lowestCost = costs[0].cost;
//			printf("costs size: %d, lowest cost: %f\n", (int)costs.size(), lowestCost);
//
//			geneticOperations(&dna, costs);
//
//			if (gen == generations - 1)
//				bestDna = dna[costs[0].species];
//		}
//	}
//
//	CPU_T
//		gene::GeneticAssemblyRatz<cpu_t>::GeneticAssemblyRatz(cpu::CpuSimulator<cpu_t>* cpu) {
//		this->cpu = cpu;
//		program = new vector<unsigned char>(maxProgLength, (unsigned char)0);
//	}
//
//	CPU_T
//		dna_t gene::GeneticAssemblyRatz<cpu_t>::getRandomSingleDna() {
//		dna_t dna;
//		string op = getRandomOp();
//		OpArgument<cpu_t> left = getRandomLeft(op);
//		OpArgument<cpu_t> right = getRandomRight(op, left.type);
//		string leftString = (left.type == NOTHING ? "" : getPrefix(left.type) + to_string(left.val));
//		string rightString = (right.type == NOTHING ? "" : getPrefix(right.type) + to_string(right.val));
//		dna.push_back("\n" + op + " " + leftString + " " + rightString);
//		return dna;
//	}
//
//	CPU_T
//		void gene::GeneticAssemblyRatz<cpu_t>::mutateOpArgValLeft(string op, cpu::OpArgument<cpu_t>* left) {
//		MemoryType argType = left->type;
//		MemoryTypeRange range = (*cpu).bruhMap.count({ op,argType }) ? (*cpu).bruhMap.at({ op,argType }) : (*cpu).rangesMap.at(argType);
//
//		//this is fugly and probably doesnt work lets gooooooooo
//		cpu_t min = range.min > left->val - params.mutateRange ? range.min : left->val - params.mutateRange;
//		cpu_t max = range.max < params.mutateRange + left->val ? range.max : params.mutateRange + left->val;
//
//		left->val = (int)drand((long long)max - (long long)min + 1) + (long long)min;
//	}
//
//	CPU_T
//		void gene::GeneticAssemblyRatz<cpu_t>::mutateOpArgValRight(string op, cpu::OpArgument<cpu_t>* left, cpu::OpArgument<cpu_t>* right) {
//		MemoryType argType = right->type;
//		MemoryTypeRange range = (*cpu).bruhMap2.count({ op, left->type, argType }) ? (*cpu).bruhMap2.at({ op, left->type, argType }) : (*cpu).rangesMap.at(argType);
//
//		//this is fugly and probably doesnt work lets gooooooooo
//		cpu_t min = range.min >= ((long long)right->val - (long long)params.mutateRange) ? range.min : right->val - params.mutateRange;
//		cpu_t max = range.max <= ((long long)params.mutateRange + (long long)right->val) ? range.max : params.mutateRange + right->val;
//
//		right->val = (int)drand((long long)max - (long long)min + 1) + (long long)min;
//	}
//
//	//TODO: test this to see if it needs improvements
//	CPU_T
//		double gene::GeneticAssemblyRatz<cpu_t>::dnaDifference(dna_t dna1, dna_t dna2) {
//		vector<int> dna1Ops;
//		vector<int> dna2Ops;
//		vector<pair<OpArgument<cpu_t>, OpArgument<cpu_t> >> dna1ArgPairs;
//		vector<pair<OpArgument<cpu_t>, OpArgument<cpu_t> >> dna2ArgPairs;
//
//		for (int i = 0; i < dna1.size(); i++) {
//			dna1Ops.push_back(str_to_baseOp(splitString(&dna1[i], ' ')[0]));
//			dna1ArgPairs.push_back(str_to_args<cpu_t>(dna1[i]));
//		}
//		for (int i = 0; i < dna2.size(); i++) {
//			dna2Ops.push_back(str_to_baseOp(splitString(&dna2[i], ' ')[0]));
//			dna2ArgPairs.push_back(str_to_args<cpu_t>(dna2[i]));
//		}
//
//		vector<pair<OpArgument<cpu_t>, OpArgument<cpu_t>>>* bigger = &dna2ArgPairs;
//		vector<pair<OpArgument<cpu_t>, OpArgument<cpu_t>>>* smaller = &dna1ArgPairs;
//		if (dna1ArgPairs.size() >= dna2ArgPairs.size()) {
//			bigger = &dna1ArgPairs;
//			smaller = &dna2ArgPairs;
//		}
//		double sum = 0;
//		for (int i = 0; i < (smaller->size()); i++) {
//			if (dna1Ops[i] != dna2Ops[i])sum++;
//			if (smaller->at(i).first.type != bigger->at(i).first.type
//				|| smaller->at(i).second.type != bigger->at(i).second.type)sum++;
//		}
//
//		//i chose 2.7 cus i think thats around the average op size
//		sum += (double)2.7 * (double)((bigger->size()) - (smaller->size()));
//		return sum;
//	}
//
//	CPU_T
//		vector<int> gene::GeneticAssemblyRatz<cpu_t>::getResult(vector<int> inputs) {
//		applyInput(inputs);
//		(*cpu).runProgram(&program->at(0), progLength);
//		vector<int> registers;
//		for (int i = 0; i < 16; i++) {
//			registers.push_back(cpu->getRegister(i));
//		}
//		return registers;
//	}
//
//	//yes
//	CPU_T
//		void gene::GeneticAssemblyRatz<cpu_t>::cost(double* currentCost, vector<int> expected, vector<int> result) {
//		double cost = 0;
//		for (int i = 0; i < result.size(); i++) {
//			cost += abs(expected[i] - result[i]);
//		}
//		*currentCost = cost;
//	}
//
//	CPU_T
//		void gene::GeneticAssemblyRatz<cpu_t>::postIterateCost(double* currentCost, vector<vector<int>> x) {
//		*currentCost = (*currentCost) / ((double)x.size());
//	}
//
//	//i guess we will use entropy idk man
//	//i wanna use this for stuff like NEAT where we have different species
//	//also maybe use this for a different selection where the unique survive
//
//	CPU_T
//		void gene::GeneticAssemblyRatz<cpu_t>::geneticOperations(vector<dna_t>* dna, vector<costInfo> costs) {
//		//selection
//		//im gonna use tournament selection cus i havent tried that yet
//		//idk kinda cringe to implemet jk its ez mode lets go bois
//		vector<dna_t> init = vector<dna_t>(*dna);//copy of dna
//		vector<pair<dna_t, int>> population;
//		//BruhIterator iter(&population);
//
//		for (int k = 0; k < params.tournamentK; k++) {
//			int pick = irand((init.size()) - 1) + 1;
//			bool found = false;
//			auto findFn = [&](pair<dna_t, int> val) {
//				if (val.second == pick)
//					found = true;
//			};
//			for_each(population.begin(), population.end(), findFn);
//			if (found) {
//				k--;
//				continue;
//			}
//
//			printf("pick: %d, init size: %d\n", pick, (int)init.size());
//			init.erase(init.begin() + pick);
//			population.push_back({ dna->at(pick), pick });
//			//for (int i = 0; i < population.size(); i++) {
//			//	int *index = &(population[i].second);
//			//	if (*index >= pick-1) (*index)--;
//			//}
//		}
//
//		//sort(costs);
//		//for () {
//
//		//}
//
//
//		//mutate possibilities
//		//-change op into another instruction
//		//-change one of params into one of the possibilites defined in some array idk
//		//-delete an op
//		//-create new op with random params
//		//-do numerical operation on one of params
//		//
//		//params:
//		//	mutatePercent:			how likely an opcode is to undergo mutation
//		for (int i = 0; i < (population.size()); i++) {//iterating throuch the list of programs
//			dna_t currentDna = population[i].first;
//			if (frand(1) <= params.mutateNewOpChance) {//add new code to program
//				currentDna.push_back(getRandomSingleDna()[0]);
//			}
//			for (int j = 0; j < currentDna.size(); j++) {//iterating inside a singular program
//				if (frand(1) <= params.mutatePercent) {
//					printf("mutate\n");
//					vector<string> split = splitString(&currentDna[j], ' ');
//					string op = "";
//					OpArgument<cpu_t> leftArg;
//					OpArgument<cpu_t> rightArg;
//					bool leftInit = false;
//					bool rightInit = false;
//					switch (split.size()) {
//					case 0:
//						break;//lol wat
//					case 1:
//						op = split[0];
//						leftArg = { NOTHING, 0 };
//						rightArg = { NOTHING, 0 };
//						break;
//					case 2:
//						op = split[0];
//						leftInit = true;
//						leftArg = string_to_arg<cpu_t>(split[1]);
//						rightArg = { NOTHING, 0 };
//						break;
//					case 3:
//						op = split[0];
//						leftInit = true;
//						leftArg = string_to_arg<cpu_t>(split[1]);
//						rightInit = true;
//						rightArg = string_to_arg<cpu_t>(split[2]);
//						break;
//					}
//
//					string mutatedDna;
//
//					if (leftInit && frand(1) <= params.mutateLeftThreshold) {
//						if (frand(1) <= params.mutateOpThreshold) {
//							currentDna[j] = getRandomSingleDna()[0];
//							mutatedDna = currentDna[j];
//							goto writeOp;
//						}
//						if (frand(1) <= params.mutateLeftType)leftArg.type = getRandomLeftType(op);
//						mutateOpArgValLeft(op, &leftArg);
//						mutatedDna = "\n" + op + " " + arg_to_string(leftArg) + " " + arg_to_string(rightArg);
//						goto writeOp;
//					}
//					if (rightInit && frand(1) <= params.mutateRightType)
//						rightArg.type = getRandomRightType(op, leftArg.type);
//					if (rightInit)
//						mutateOpArgValRight(op, &leftArg, &rightArg);
//
//					mutatedDna = "\n" + op + " " + arg_to_string(leftArg) + " " + arg_to_string(rightArg);
//				writeOp:
//					printf("dna size: %d, j: %d\n", (*dna)[population[i].second].size(), j);
//
//					//sometimes j goes out of range?
//					(*dna)[population[i].second][j] = mutatedDna;//TODO: error divergence is > 1 here
//				}
//			}
//		}
//		//crossover possibilities
//		//get some code and then switch some lines
//		//
//		//params:
//		//	crossoverPercent:       how liekly crossover is to happen
//		//	crossoverLengthMax:     maximum lines to get switched
//		//	crossoverLengthMin:     minimum lines to get switched
//	}
//
//	//random_selector<> selector{};
//
//	CPU_T
//		string gene::GeneticAssemblyRatz<cpu_t>::getRandomOp() {
//		int size = opMap.size();
//		int num = irand(size);//choose between 0 and size-1;
//
//		/*while (true) {//slow but we are going for da dev speed rn
//			for (auto iter = opMap.begin(); iter != opMap.end(); ++iter) {
//				float num = (float)drand(1);
//				if (frand(1) <= (1.0 / (float)size))return string(iter->first);
//			}
//		}*/
//		int count = 0;
//		for (auto iter = opMap.begin(); iter != opMap.end(); ++iter) {
//			if (count == num)return string(iter->first);
//			count++;
//		}
//	}
//
//	CPU_T
//		OpArgument<cpu_t> gene::GeneticAssemblyRatz<cpu_t>::getRandomLeft(string op) {
//		OpArgCombos opArgsCombo = opArgs.at(removeSpace(op));
//		vector<MemoryType>* possibleArgs = &opArgsCombo.left;
//
//		if (possibleArgs->size() <= 0)return { NOTHING, 0 };
//
//		MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());
//		MemoryTypeRange range = (*cpu).bruhMap.count({ removeSpace(op),argType }) ? (*cpu).bruhMap.at({ removeSpace(op),argType }) : (*cpu).rangesMap.at(argType);
//
//		return { argType, (cpu_t)drand(range.max - range.min + 1) + range.min };
//	}
//
//
//
//	CPU_T
//		MemoryType gene::GeneticAssemblyRatz<cpu_t>::getRandomLeftType(string op) {
//		OpArgCombos opArgsCombo = opArgs.at(removeSpace(op));
//		vector<MemoryType>* possibleArgs = &opArgsCombo.left;
//
//		if (possibleArgs->size() <= 0)return NOTHING;
//
//		MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());
//
//		return argType;
//	}
//
//	//cpu::OpArgument<cpu_t> gene::GeneticAssemblyRatz::getRandomRight(string op, cpu::MemoryType left)
//	//{
//	//	return cpu::OpArgument<cpu_t>();
//	//}
//
//	CPU_T
//		cpu::OpArgument<cpu_t> gene::GeneticAssemblyRatz<cpu_t>::getRandomRight(string op, cpu::MemoryType left) {
//		OpArgCombos opArgsCombo = opArgs.at(removeSpace(op));
//		vector<MemoryType>* possibleArgs = &opArgsCombo.right;
//
//		if (possibleArgs->size() <= 0)return { NOTHING, 0 };
//
//		MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());
//		MemoryTypeRange range = (*cpu).bruhMap2.count({ removeSpace(op),left,argType }) ? (*cpu).bruhMap2.at({ removeSpace(op),left,argType }) : (*cpu).rangesMap.at(argType);
//
//		return { argType, (cpu_t)drand(range.max - range.min + 1) + range.min };
//	}
//
//	CPU_T
//		cpu::MemoryType gene::GeneticAssemblyRatz<cpu_t>::getRandomRightType(string op, cpu::MemoryType left) {
//		printf("op: '%s'\n", removeSpace(op.c_str()));
//		OpArgCombos opArgsCombo = opArgs.at(removeSpace(op));
//		vector<MemoryType>* possibleArgs = &opArgsCombo.right;
//
//		if (possibleArgs->size() <= 0)return NOTHING;
//
//		MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());
//
//		return argType;
//	}
//
//	CPU_T
//		dna_t gene::GeneticAssemblyRatz<cpu_t>::getRandomDna() {
//		dna_t dna;
//		do {
//			dna.push_back(getRandomSingleDna()[0]);
//		} while (frand(1) <= params.newDnaChance);
//		return dna;
//	}
//
//	CPU_T
//		void gene::GeneticAssemblyRatz<cpu_t>::applyInput(vector<int> inputs) {//the registers? i think lol i forgot
//		(*cpu).resetRegisters();
//		(*cpu).resetMemory();
//		for (int i = 0; i < inputs.size(); i++) {
//			(*cpu).setMemory(i, inputs[i]);
//		}
//	}
//
//	CPU_T
//		void gene::GeneticAssemblyRatz<cpu_t>::applyDna(dna_t dna) {//the code
//		string code;
//		for (int i = 0; i < dna.size(); i++) {
//			string str = dna[i];
//			code.append(str);
//		}
//		//printf("%s\n", code.c_str());
//		progLength = (*cpu).assemble(&program->at(0), code.c_str(), code.size());
//	}
//
//	//void DONT_CALL_thisExistsToAvoidLinkingError2() {
//	//	GeneticAssemblyRatz<short> bruh(nullptr);
//	//	GeneticAssemblyRatz<int> bruh2(nullptr);
//	//	GeneticAssemblyRatz<long> bruh3(nullptr);
//	//	GeneticAssemblyRatz<long long> bruh4(nullptr);
//
//	//	compare({ 0, 0.0 }, { 0, 0.0 });
//	//}
//
//	//template GeneticAssemblyRatz<short>;
//	//template GeneticAssemblyRatz<int>;
//	//template GeneticAssemblyRatz<long>;
//	//template GeneticAssemblyRatz<long long>;
//
////#endif
}