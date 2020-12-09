#pragma once

#ifndef MAX_PROGLENGTH
#define MAX_PROGLENGTH 0xFFFF
#endif

//#ifndef GENETIC_H
//#define GENETIC_H

//#define CPU_T template<class cpu_t>
#define DNA_T template <class dna_t>

//#include "Cpu2.h"
#include "CpuThreading.h"
//#include <algorithm>
#include <cstdarg>
#include <assert.h>
////////////////

using namespace std;//bruh ew get those stds out of here
using namespace cpu;
//TODO: cure cancer

/*TODO: some ideas
	-sub routines are added and used conditionally.
	-sub routines are added parts of it are used on condition.
	-sub routines are added and the conditions to use it are described using a graph
	-sub routines are added and the conditions to use it are described using a
*/

/*
list of bruh moments:
or mem r-40
or ptr r-8
or r12 r-1
add r6 f-29
or mem r-8
add ptr r-5
and mem r-10
and ptr r-10
or mem r-66
*/

typedef vector<string> dna_t;//yooooo letsss gooooo, lol stds

namespace gene {
	struct costInfo {
		int species;
		double cost;
	};

	bool compare(costInfo i, costInfo j);

	DNA_T
	class GeneticAlgorithm {
	protected:
		double lowestCost = 0;

		dna_t bestDna;

		int maxSpecies = 0;

		virtual vector<int> getResult(vector<int>* inputs) = 0;
		virtual void cost(double* currentCost, vector<int> expected, vector<int> result) = 0;
		virtual void postIterateCost(double* currentCost, vector<vector<int>> x) = 0;
		virtual double dnaDifference(dna_t dna1, dna_t dna2) = 0;
		virtual void geneticOperations(vector<dna_t>* dna, vector<costInfo> costs) = 0;
		virtual void applyInput(vector<int>* inputs) = 0;
		virtual void applyDna(dna_t dna) = 0;
		virtual dna_t getRandomDna() = 0;
	public:
		bool debug = false;
		void train(int generations, int maxSpecies, vector<vector<int>> x, vector<vector<int>> y) {
			vector<dna_t> dna;

			for (int species = 0; species < maxSpecies; species++) {
				dna_t dnaa = getRandomDna();
				log("init species %d size: %d, '%s'\n\n", species, dnaa.size(), combine((vector<string>)dnaa).c_str());
				dna.push_back(dnaa);
			}

			//TEST TO SEE IF COST IS WORKING
			//dna_t solutionDna = {"\nmov f2 1", "\nmov f3 1", "\nmov ptr 0", "\nmov r0 mem"};
			//dna[0] = solutionDna;
			//printf("solutionDna: '%s'\n", combine((vector<string>)solutionDna).c_str());

			assert(dna.size() == maxSpecies);

			for (int gen = 0; gen < generations; gen++) {
				printf("Generation %d, ", gen);
				vector<costInfo> costs(maxSpecies, { 0,0 });
				for (int species = 0; species < maxSpecies; species++) {
					log("Species_Start: %d\n", species);
					//printf("'%s'\n", combine((vector<string>)dna[species]).c_str());
					//printf("Code:%s\n\n", combine(dna[species]).c_str());
					double cost = 0;
					applyDna(dna[species]);
					//if (species == 0) {
					//	printf("solutionDna: '%s'\n", combine((vector<string>)dna[species]).c_str());
					//}
					//log("applied dna\n");
					for (int i = 0; i < x.size(); i++) {
						//log("Apply input: %d, Input: %s\n", i, array_to_string<int>(&x[i][0], x[i].size()).c_str());
						applyInput(&x[i]);
						//log("applied input\n");

						//printf("i: %d\n", i);
						auto expected = y[i];
						//log("expected: %s\n", array_to_string(&expected[0], expected.size()).c_str());
						
						auto result = getResult(&x[i]);
						//log("result: %s\n", array_to_string(&result[0], result.size()).c_str());

						this->cost(&cost, expected, result);
						log("calculated cost: %f\n\n", cost);
					}
					double preCost = cost;

					postIterateCost(&cost, x);

					costs[species].species = species;
					costs[species].cost = cost;
					//totalCost += cost;
					//printf("Species_End: %d cost: %f\n\n", species, cost);

				}
				std::sort(costs.begin(), costs.end(), compare);

				lowestCost = costs[0].cost;
				// printf("costs size: %d, lowest cost: %f\n", (int)costs.size(), lowestCost);
				float avg = 0;
				const int dnaLength = dna.size();
				for(int a = 0; a < dnaLength; a++){
					avg += dna[a].size();
				}
				avg /= (float)dnaLength;
				printf("average program length: %f\n", avg);
				geneticOperations(&dna, costs);

				if (gen == generations - 1)
					bestDna = dna[costs[0].species];

				if (lowestCost <= 0.01 && bestDna.size() != 0) {
					printf("prog length: %d, '%s'\n", ((vector<string>)bestDna).size(), combine((vector<string>)bestDna).c_str());
					return;
				}
			}
		}

	protected:
		void log(const char* str, ...) {
			if (debug) {
				va_list argptr;
				va_start(argptr, str);
				vfprintf(stderr, str, argptr);
				va_end(argptr);
			}
		}
		//template <class type>
		//static void print_array(type data, int length);

		//template <class type>
		//static void print_array(type data, int length, char separator);
	};

	struct GeneticAssemblyParams {
		int tournamentK = 3;

		float mutatePercent = 0.3f;
		int mutateRange = 10;//like how much it can add or sub
		float mutateLeftThreshold = 0.2f;
		float mutateOpThreshold = 0.2f;
		float mutateRightType = 0.1f;
		float mutateLeftType = 0.1f;
		float mutateNewOpChance = 0.5f;
		float mutateDeleteOpChance = 0.05f;

		float crossOverPercent = 0.3f;
		int crossOverMax = 10;
		int crossOverMin = 1;

		float newDnaChance = 0.9f;//this makes the randomDna return longer dna strands
		float deleteDnaChance = 0.1f;

		int maxProgTime = 1;//max amount of time a program can last in milliseconds
	};

	//ill learn this stuff eventually bruv dont worry atleast im giving credit
		CPU_TEMPLATE
		class GeneticAssemblyRatz : public GeneticAlgorithm<dna_t> {
		public:
			GeneticAssemblyParams params;

			GeneticAssemblyRatz(cpu::CpuSimulator<cpu_t>* cpu) {
				this->cpu = cpu;
				progTrashEdition = vector<unsigned char>(MAX_PROGLENGTH, (unsigned char)0);
				program = &progTrashEdition;
				//this->program = &progTrashEdition;
				//this->cpuThread = cpu::CpuRunProgramThread<cpu_t>(cpu);
				//printf("cpuThread in genetic: %p\n", &(this->cpuThread));
			}
			
			dna_t getRandomSingleDna() {
				dna_t dna;
				string op = getRandomOp();
				OpArgument<cpu_t> left = getRandomLeft(op);
				OpArgument<cpu_t> right = getRandomRight(op, left.type);
				dna.push_back("\n" + op + " " + arg_to_string(left) + " " + arg_to_string(right));
				return dna;
			}

			void mutateOpArgValLeft(string op, cpu::OpArgument<cpu_t>* left) {
				MemoryType argType = left->type;
				MemoryTypeRange<cpu_t> range = (*cpu).bruhMap.count({ op,argType }) ? (*cpu).bruhMap.at({ op,argType }) : (*cpu).rangesMap.at(argType);

				//this is fugly and probably doesnt work lets gooooooooo
				//cpu_t min = range.min > left->val - params.mutateRange ? range.min : left->val - params.mutateRange;
				//cpu_t max = range.max < params.mutateRange + left->val ? range.max : params.mutateRange + left->val;

				cpu_t rangeStart = left->val - params.mutateRange;
				cpu_t rangeEnd = left->val + params.mutateRange;

				cpu_t _min = std::max(range.min, rangeStart);
				cpu_t _max = std::min(range.max, rangeEnd);

				left->val = (drand() * (_max - _min)) + _min;

				//left->val = (int)drand((long long)max - (long long)min + 1) + (long long)min;
			}

			void mutateOpArgValRight(string op, cpu::OpArgument<cpu_t>* left, cpu::OpArgument<cpu_t>* right) {
				MemoryType argType = right->type;
				MemoryTypeRange<cpu_t> range = (*cpu).bruhMap2.count({ op, left->type, argType }) ? (*cpu).bruhMap2.at({ op, left->type, argType }) : (*cpu).rangesMap.at(argType);

				////this is fugly and probably doesnt work lets gooooooooo
				//cpu_t min = range.min >= ((long long)right->val - (long long)params.mutateRange) ? range.min : right->val - params.mutateRange;
				//cpu_t max = range.max <= ((long long)params.mutateRange + (long long)right->val) ? range.max : params.mutateRange + right->val;

				cpu_t rangeStart = right->val - params.mutateRange;
				cpu_t rangeEnd = right->val + params.mutateRange;

				cpu_t _min = std::max(range.min, rangeStart);
				cpu_t _max = std::min(range.max, rangeEnd);

				right->val = (drand() * (_max - _min)) + _min;

				//right->val = (int)drand((long long)max - (long long)min + 1) + (long long)min;
				//right->val = between(range.min, range.max, params.mutateRange+right->val);
			}

			//TODO: test this to see if it needs improvements
			double dnaDifference(dna_t dna1, dna_t dna2) {
				vector<int> dna1Ops;
				vector<int> dna2Ops;
				vector<pair<OpArgument<cpu_t>, OpArgument<cpu_t> >> dna1ArgPairs;
				vector<pair<OpArgument<cpu_t>, OpArgument<cpu_t> >> dna2ArgPairs;

				for (int i = 0; i < dna1.size(); i++) {
					dna1Ops.push_back(str_to_baseOp(splitString(&dna1[i], ' ')[0]));
					dna1ArgPairs.push_back(str_to_args<cpu_t>(dna1[i]));
				}
				for (int i = 0; i < dna2.size(); i++) {
					dna2Ops.push_back(str_to_baseOp(splitString(&dna2[i], ' ')[0]));
					dna2ArgPairs.push_back(str_to_args<cpu_t>(dna2[i]));
				}

				vector<pair<OpArgument<cpu_t>, OpArgument<cpu_t>>>* bigger = &dna2ArgPairs;
				vector<pair<OpArgument<cpu_t>, OpArgument<cpu_t>>>* smaller = &dna1ArgPairs;
				if (dna1ArgPairs.size() >= dna2ArgPairs.size()) {
					bigger = &dna1ArgPairs;
					smaller = &dna2ArgPairs;
				}
				double sum = 0;
				for (int i = 0; i < (smaller->size()); i++) {
					if (dna1Ops[i] != dna2Ops[i])sum++;
					if (smaller->at(i).first.type != bigger->at(i).first.type
						|| smaller->at(i).second.type != bigger->at(i).second.type)sum++;
				}

				double opAverageLength = 2.7;//TODO: calculate this?
				sum += opAverageLength * (double)((bigger->size()) - (smaller->size()));
				return sum;
			}

		protected:
			const int maxProgLength = 0xFFFF;     //why not idk man
			int progLength = 0;
			vector<unsigned char> progTrashEdition;
			vector<unsigned char>* program;
			cpu::CpuSimulator<cpu_t>* cpu;
			// cpu::CpuRunProgramThread<cpu_t> cpuThread = cpu::CpuRunProgramThread<cpu_t>(&cpu);
			cpu::CpuRunProgramThread<cpu_t> cpuThread { &cpu };

			vector<int> getResult(vector<int>* inputs) {
				//printf("getresult start, proglength: %d\n", progLength);
				applyInput(inputs);//idk mannnn
				cpuThread.runProgram(&program->at(0), progLength);
				if (!cpuThread.sleepUntilComplete(params.maxProgTime))log("took too long\n");

				//cpu->dumpRegisters();

				//printf("ran program\n");
				vector<int> registers;
				for (int i = 0; i < 16; i++) {
					registers.push_back(cpu->getRegister(i));
				}
				//printf("getresult finish\n");
				return registers;
			}

			//yes
			void cost(double* currentCost, vector<int> expected, vector<int> result) {
				double cost = 0;
				for (int i = 0; i < result.size(); i++) {
					cost += abs(expected[i] - result[i]);
				}
				*currentCost = cost;
			}

			void postIterateCost(double* currentCost, vector<vector<int>> x) {
				*currentCost = (*currentCost) / ((double)x.size());
			}

			//i guess we will use entropy idk man
			//i wanna use this for stuff like NEAT where we have different species
			//also maybe use this for a different selection where the unique survive

			void geneticOperations(vector<dna_t>* dna, vector<costInfo> costs) {
				//selection
				//im gonna use tournament selection cus i havent tried that yet
				//idk kinda cringe to implemet jk its ez mode lets go bois
				// vector<dna_t> init = vector<dna_t>(*dna);//copy of dna
				// vector<pair<dna_t, int>> population;
				vector<pair<dna_t*, int>> population;
				
				//BruhIterator iter(&population);

				for (int i = 0; i < dna->size(); i++) {
					population.push_back({&dna->at(i), i});
				}

				std::shuffle(population.begin(), population.end(), std::default_random_engine(time(NULL)));

				auto pBegin = population.begin();
				for (int k = 1; k <= (population.size() - params.tournamentK); k++) {
					population.erase(pBegin + k);
				}

				for (int i = 0; i < (population.size()); i++) { //iterating throuch the list of programs
					pair<dna_t*, int>* currentDnaPtr = &population[i];
					dna_t* realDnaPtr = &((*dna)[currentDnaPtr->second]);

					for (int j = 0; j < /*currentDna.size()*/realDnaPtr->size()-1; j++) { //iterating inside a singular program
						
						//dna_t currentDna = currentDnaPtr->first;
						if (frand(1) <= params.mutatePercent) {
							if (frand(1) <= params.mutateNewOpChance) {//add new code to program
								//currentDna.insert(currentDna.begin() + j, getRandomSingleDna()[0]);
								realDnaPtr->insert(realDnaPtr->begin() + j, getRandomSingleDna()[0]);
								j--;
								//printf("added one\n");
								continue;
							}
							if (frand(1) <= params.mutateDeleteOpChance) {
								//currentDna.erase(currentDna.begin() + j);
								realDnaPtr->erase(realDnaPtr->begin() + j);
								j++;
								//printf("erased one\n");
								continue;
							}

							log("mutate species %d:\n", population[i].second);
							//printf("mutate start\n");
							vector<string> split = splitString(&realDnaPtr->at(j), ' ');
							//vector<string> split = splitString(&currentDna[j], ' ');
							for (int z = 0; z < split.size();z++) {
								split[z] = removeSpace(split[z]);
							}
							string op = "";
							OpArgument<cpu_t> leftArg;
							OpArgument<cpu_t> rightArg;
							bool leftInit = false;
							bool rightInit = false;
							switch (split.size()) {
							case 0:
								printf("WARNING in Genetic.h: split size was zero\n");
								break;//lol wat
							case 1:
								op = split[0];
								leftArg = { NOTHING, -1 };
								rightArg = { NOTHING, -1 };
								break;
							case 2:
								op = split[0];
								leftInit = true;
								leftArg = string_to_arg<cpu_t>(split[1]);
								rightArg = { NOTHING, -1 };
								break;
							case 3:
								op = split[0];
								leftInit = true;
								leftArg = string_to_arg<cpu_t>(split[1]);
								rightInit = true;
								rightArg = string_to_arg<cpu_t>(split[2]);
								break;
							}

							string mutatedDna;

							if (frand(1) <= params.mutateOpThreshold) {
								realDnaPtr->at(j) = getRandomSingleDna()[0];
								//currentDna[j] = getRandomSingleDna()[0];
								mutatedDna = realDnaPtr->at(j);
								//mutatedDna = currentDna[j];
								log("MUT_OP\n");
								goto writeOp;
							}

							//printf("op: '%s',",op.c_str());
							if (leftInit && frand(1) <= params.mutateLeftThreshold) {
								//printf("mutate op: '%s'\n", op.c_str());
								if (leftInit && frand(1) <= params.mutateLeftType) {
									leftArg.type = getRandomLeftType(op);
									log("MUT_LEFT_TYPE\n");
								}
								mutateOpArgValLeft(op, &leftArg);
								mutatedDna = "\n" + op + " " + arg_to_string(leftArg) + " " + arg_to_string(rightArg);
								log("MUT_LEFT_VAL\n");
								goto writeOp;
							}
							if (rightInit && frand(1) <= params.mutateRightType) {
								rightArg.type = getRandomRightType(op, leftArg.type);
								log("MUT_RIGHT_TYPE\n");
							}
							if (rightInit) {
								mutateOpArgValRight(op, &leftArg, &rightArg);
								log("MUT_RIGHT_VAL\n");
							}

							log("op: %s, leftArg: %d, rightArg: %d\n", op.c_str(),
											static_cast<int>(leftArg.type), static_cast<int>(rightArg.type));
							mutatedDna = "\n" + op + " " + arg_to_string(leftArg) + " " + arg_to_string(rightArg);
						writeOp:
							//printf("dna size: %d, j: %d\n", (*dna)[population[i].second].size(), j);

							realDnaPtr->at(j) = mutatedDna;
							//(*dna)[population[i].second][j] = mutatedDna;
							log("writeOp\n");
						}
					}

				}
				
				//TODO: implement the ability to use crossOver parameters from params variable
				for (int i = 0; i < (population.size()); i++) {
					if (frand(1) <= params.crossOverPercent) { //crossover baby
						pair<dna_t*, int>* currentDna = &population[i];
						dna_t* realCurrentDna = &((*dna)[currentDna->second]);

						pair<dna_t*, int>* selectedDna = &population[irand(population.size() - 1)];
						dna_t* realSelectedDna = &((*dna)[selectedDna->second]);

						const int minProgSize = min(realSelectedDna->size(), realCurrentDna->size());
						const int startPoint = irand(minProgSize - 1);
						const int endPoint = irand(minProgSize-startPoint)+startPoint;

						//const int maxLines = min(currentDna->size(), min(params.crossOverMax, selectedDna->size()));
						//const int minLines = min(maxLines, params.cross);

						////select random starting point
						//const int lines = irand(maxLines-minLines)+minLines;

						//we are going to continue until we have either reached the end or reached max lines
						for (int j = startPoint; j < endPoint; j++) {
							std::string temp = std::string((*realCurrentDna)[j]);
							realCurrentDna->at(j) = (*realSelectedDna)[j];
							realSelectedDna->at(j) = temp;
						}
					}
				}
			}

			random_selector<> selector{};

			string getRandomOp() {
				int size = opMap.size();
				int num = irand(size);//choose between 0 and size-1;

				/*while (true) {//slow but we are going for da dev speed rn
					for (auto iter = opMap.begin(); iter != opMap.end(); ++iter) {
						float num = (float)drand(1);
						if (frand(1) <= (1.0 / (float)size))return string(iter->first);
					}
				}*/
				int count = 0;
				for (auto iter = opMap.begin(); iter != opMap.end(); ++iter) {
					if (count == num)return string(iter->first);
					count++;
				}
				return "";
			}

			cpu::OpArgument<cpu_t> getRandomLeft(string op) {
				const std::string strippedOp = removeSpace(op);
				OpArgCombos opArgsCombo = opArgs.at(strippedOp);
				vector<MemoryType>* possibleArgs = &opArgsCombo.left;

				if (possibleArgs->size() <= 0)return { NOTHING, 0 };

				MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());
				//printf("op: %s, memType: %d\n", op.c_str(), static_cast<int>(argType));
				MemoryTypeRange<cpu_t> range = (*cpu).bruhMap.count({ strippedOp,argType }) ? (*cpu).bruhMap.at({ strippedOp,argType }) : (*cpu).rangesMap.at(argType);

				return { argType, (cpu_t)drand(range.max - range.min + 1) + range.min };
			}

			cpu::MemoryType getRandomLeftType(string op) {
				const std::string strippedOp = removeSpace(op);
				OpArgCombos opArgsCombo = opArgs.at(strippedOp);
				vector<MemoryType>* possibleArgs = &opArgsCombo.left;

				if (possibleArgs->size() <= 0)return NOTHING;

				MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());

				return argType;
			}

			cpu::OpArgument<cpu_t> getRandomRight(string op, cpu::MemoryType left) {
				const std::string strippedOp = removeSpace(op);
				OpArgCombos opArgsCombo = opArgs.at(strippedOp);
				vector<MemoryType>* possibleArgs = &opArgsCombo.right;

				if (possibleArgs->size() <= 0)return { NOTHING, 0 };

				MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());
				MemoryTypeRange<cpu_t> range = (*cpu).bruhMap2.count({ strippedOp,left,argType }) ? (*cpu).bruhMap2.at({ strippedOp,left,argType }) : (*cpu).rangesMap.at(argType);

				return { argType, (cpu_t)drand(range.max - range.min + 1) + range.min };
			}

			cpu::MemoryType getRandomRightType(string op, cpu::MemoryType left) {
				//printf("op: '%s'\n", removeSpace(op.c_str()).c_str());
				OpArgCombos opArgsCombo = opArgs.at(removeSpace(op));
				vector<MemoryType>* possibleArgs = &opArgsCombo.right;

				if (possibleArgs->size() <= 0)return NOTHING;

				MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());

				return argType;
			}

			dna_t getRandomDna() {
				dna_t dna;
				do {
					dna.push_back(getRandomSingleDna()[0]);
				} while (frand(1) <= params.newDnaChance);
				return dna;
			}
			void applyInput(vector<int>* inputs) {//the registers? i think lol i forgot
				cpu->reset();
				for (int i = 0; i < inputs->size(); i++) {
					if (cpu->setMemory(i, inputs->at(i))) {
						log("set memory\n");
					}
					else {
						log("failed to set memory\n");
					}
				}
				//cpu->dumpMemory();
			}
			void applyDna(dna_t dna) {//the code
				std::fill(program->begin(), program->end(), 0);//zero the program vector for good luck
				string code;
				for (int i = 0; i < dna.size(); i++) {
					string str = dna[i];
					code.append(str);
				}
				//printf("%s\n", code.c_str());
				progLength = (*cpu).assemble(&program->at(0), code.c_str(), code.size());
				//printf("assembled progLength: %d\n", progLength);
				if (progLength == 0) {
					printf("'%s'\n", code.c_str());
					printf("we got a keeper here boys\n");
					getchar();
				}
			}
	};

}

//#include "Genetic.cpp"

//#endif // !GENETIC_PROTECTION