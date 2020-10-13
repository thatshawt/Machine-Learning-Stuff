#pragma once
//#ifndef GENETIC_H
//#define GENETIC_H

//#define CPU_T template<class cpu_t>
#define DNA_T template <class dna_t>

//#include "Cpu2.h"
#include "CpuThreading.cpp"
//#include <algorithm>
#include <cstdarg>
#include <assert.h>

using namespace std;//bruh ew get those stds out of here
using namespace cpu;
//TODO: remove the stds because we want clean code only ok bois

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

			assert(dna.size() == maxSpecies);

			for (int gen = 0; gen < generations; gen++) {
				printf("Generation %d\n", gen);
				vector<costInfo> costs(maxSpecies, { 0,0 });
				for (int species = 0; species < maxSpecies; species++) {
					log("Species_Start: %d\n", species);
					//printf("Code:%s\n\n", combine(dna[species]).c_str());
					double cost = 0;
					applyDna(dna[species]);
					//log("applied dna\n");
					for (int i = 0; i < x.size(); i++) {
						log("Apply input: %d, Input: %s\n",
									i, array_to_string<int>(&x[i][0], x[i].size()).c_str());
						applyInput(&x[i]);
						//log("applied input\n");

						//printf("i: %d\n", i);
						auto expected = y[i];
						log("expected: %s\n", array_to_string(&expected[0], expected.size()).c_str());
						
						auto result = getResult(&x[i]);
						log("result: %s\n", array_to_string(&result[0], result.size()).c_str());

						this->cost(&cost, expected, result);
						log("calculated cost: %f\n\n", cost);
					}
					postIterateCost(&cost, x);
					costs[species].species = species;
					costs[species].cost = cost;
					//totalCost += cost;
					//printf("Species_End: %d cost: %f\n\n", species, cost);

					if (cost <= 0.01) {
						printf("you win baws\n");
						printf("prog length: %d, '%s'\n", bestDna.size(), combine((vector<string>)bestDna).c_str());
						return;
					}
				}
				std::sort(costs.begin(), costs.end(), compare);

				lowestCost = costs[0].cost;
				printf("costs size: %d, lowest cost: %f\n", (int)costs.size(), lowestCost);

				geneticOperations(&dna, costs);

				if (gen == generations - 1)
					bestDna = dna[costs[0].species];
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

	//TODO: work this out cus i want to make some pie chart thing where every action has a chance to happen idk
	//cus rn i check them in order but i want to just pick one at random.
	//im thinking of doing some pie chart thing with the actions and just choose one at random
	/*enum GeneticAssemblyParamsENUM_TIME_BRUH {
		TOURNAMENT_K,

		MUTATE_PERCENT,
		MUTATE_RANGE,
		MUTATE_LEFT_THRESHOLD,
		MUTATE_OP_THRESHOLD,
		MUTATE_RIGHT_TYPE,
		MUTATE_LEFT_TYPE,
		MUTATE_NEW_OP_CHANCE,

		CROSSOVER_PERCENT,

		NEW_DNA,
		DELETE_DNA
	};*/

	struct GeneticAssemblyParams {
		//idk why im tired i slept like more than 12 hours rip
		int tournamentK = 3;

		float mutatePercent = 0.3f;
		int mutateRange = 10;//like how much it can add or sub
		float mutateLeftThreshold = 0.2f;
		float mutateOpThreshold = 0.2f;
		float mutateRightType = 0.1f;
		float mutateLeftType = 0.1f;
		float mutateNewOpChance = 0.05f;

		float crossOverPercent = 0.3f;

		float newDnaChance = 0.9f;//this makes the randomDna return longer dna strands
	};

	//https://stackoverflow.com/questions/14294267/class-template-for-numeric-types
	//ill learn this stuff eventually bruv dont worry atleast im giving credit
		CPU_T
		class GeneticAssemblyRatz : public GeneticAlgorithm<dna_t> {
		public:
			GeneticAssemblyParams params;

			GeneticAssemblyRatz(cpu::CpuSimulator<cpu_t>* cpu) {
				this->cpu = cpu;
				program = new vector<unsigned char>(maxProgLength, (unsigned char)0);
				//this->cpuThread = cpu::CpuRunProgramThread<cpu_t>(cpu);
				//printf("cpuThread in genetic: %p\n", &(this->cpuThread));
			}

			dna_t getRandomSingleDna() {
				dna_t dna;
				string op = getRandomOp();
				OpArgument<cpu_t> left = getRandomLeft(op);
				OpArgument<cpu_t> right = getRandomRight(op, left.type);
				string leftString = (left.type == NOTHING ? "" : getPrefix(left.type) + to_string(left.val));
				string rightString = (right.type == NOTHING ? "" : getPrefix(right.type) + to_string(right.val));
				dna.push_back("\n" + op + " " + leftString + " " + rightString);
				return dna;
			}

			void mutateOpArgValLeft(string op, cpu::OpArgument<cpu_t>* left) {
				MemoryType argType = left->type;
				MemoryTypeRange range = (*cpu).bruhMap.count({ op,argType }) ? (*cpu).bruhMap.at({ op,argType }) : (*cpu).rangesMap.at(argType);

				//this is fugly and probably doesnt work lets gooooooooo
				cpu_t min = range.min > left->val - params.mutateRange ? range.min : left->val - params.mutateRange;
				cpu_t max = range.max < params.mutateRange + left->val ? range.max : params.mutateRange + left->val;

				left->val = (int)drand((long long)max - (long long)min + 1) + (long long)min;
			}

			void mutateOpArgValRight(string op, cpu::OpArgument<cpu_t>* left, cpu::OpArgument<cpu_t>* right) {
				MemoryType argType = right->type;
				MemoryTypeRange range = (*cpu).bruhMap2.count({ op, left->type, argType }) ? (*cpu).bruhMap2.at({ op, left->type, argType }) : (*cpu).rangesMap.at(argType);

				//this is fugly and probably doesnt work lets gooooooooo
				cpu_t min = range.min >= ((long long)right->val - (long long)params.mutateRange) ? range.min : right->val - params.mutateRange;
				cpu_t max = range.max <= ((long long)params.mutateRange + (long long)right->val) ? range.max : params.mutateRange + right->val;

				right->val = (int)drand((long long)max - (long long)min + 1) + (long long)min;
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

				//i chose 2.7 cus i think thats around the average op size
				sum += (double)2.7 * (double)((bigger->size()) - (smaller->size()));
				return sum;
			}

		protected:
			const int maxProgLength = 0xFFFF;     //why not idk man
			int progLength = 0;
			vector<unsigned char>* program;
			cpu::CpuSimulator<cpu_t>* cpu;
			cpu::CpuRunProgramThread<cpu_t> cpuThread = cpu::CpuRunProgramThread<cpu_t>(&cpu);
			vector<int> getResult(vector<int>* inputs) {
				//printf("getresult start, proglength: %d\n", progLength);
				applyInput(inputs);
				//printf("apllied inputs\n");
				cpu->resetRegisters();
				//printf("reset registers from main thread\n");
				//printf("main thread id: %d\n", std::this_thread::get_id());
				cpuThread.runProgram(&program->at(0), progLength);
				if (!cpuThread.sleepUntilComplete(10))printf("took too long");

				//printf("ran program\n");
				vector<int> registers;
				for (int i = 0; i < 16; i++) {
					registers.push_back(cpu->getRegister(i));
					//printf("added register thing\n");
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
				vector<dna_t> init = vector<dna_t>(*dna);//copy of dna
				vector<pair<dna_t, int>> population;
				//BruhIterator iter(&population);

				for (int i = 0; i < init.size(); i++) {
					population.push_back({init.at(i), i});
				}

				std::shuffle(population.begin(), population.end(), std::default_random_engine(time(NULL)));

				auto pBegin = population.begin();
				for (int k = 1; k <= (population.size() - params.tournamentK); k++) {
					population.erase(pBegin + k);
				}

				//for (int k = 0; k < params.tournamentK; k++) {
				//	int pick = irand(init.size());
				//	bool found = false;
				//	auto findFn = [&](pair<dna_t, int> val) {
				//		if (val.second == pick)
				//			found = true;
				//	};
				//	for_each(population.begin(), population.end(), findFn);
				//	if (found) {
				//		printf("pick: %d, init size: %d\n", pick, init.size());
				//		k--;
				//		continue;
				//	}

				//	printf("pick: %d, init size: %d\n", pick, (int)init.size());
				//	//init.erase(init.begin() + pick);
				//	population.erase(population.begin() + pick);
				//	//population.push_back({ dna->at(pick), pick });
				//	//for (int i = 0; i < population.size(); i++) {
				//	//	int *index = &(population[i].second);
				//	//	if (*index >= pick-1) (*index)--;
				//	//}
				//}

				//sort(costs);
				//for () {

				//}


				//mutate possibilities
				//-change op into another instruction
				//-change one of params into one of the possibilites defined in some array idk
				//-delete an op
				//-create new op with random params
				//-do numerical operation on one of params
				//
				//params:
				//	mutatePercent:			how likely an opcode is to undergo mutation
				for (int i = 0; i < (population.size()); i++) {//iterating throuch the list of programs
					dna_t currentDna = population[i].first;
					if (frand(1) <= params.mutateNewOpChance) {//add new code to program
						currentDna.push_back(getRandomSingleDna()[0]);
					}
					for (int j = 0; j < currentDna.size()-1; j++) {//iterating inside a singular program
						if (frand(1) <= params.mutatePercent) {
							//printf("mutate start\n");
							vector<string> split = splitString(&currentDna[j], ' ');
							string op = "";
							OpArgument<cpu_t> leftArg;
							OpArgument<cpu_t> rightArg;
							bool leftInit = false;
							bool rightInit = false;
							switch (split.size()) {
							case 0:
								break;//lol wat
							case 1:
								op = split[0];
								leftArg = { NOTHING, 0 };
								rightArg = { NOTHING, 0 };
								break;
							case 2:
								op = split[0];
								leftInit = true;
								leftArg = string_to_arg<cpu_t>(split[1]);
								rightArg = { NOTHING, 0 };
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

							if (leftInit && frand(1) <= params.mutateLeftThreshold) {
								if (frand(1) <= params.mutateOpThreshold) {
									currentDna[j] = getRandomSingleDna()[0];
									mutatedDna = currentDna[j];
									log("MUT_OP\n");
									goto writeOp;
								}
								if (frand(1) <= params.mutateLeftType) {
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

							mutatedDna = "\n" + op + " " + arg_to_string(leftArg) + " " + arg_to_string(rightArg);
						writeOp:
							//printf("dna size: %d, j: %d\n", (*dna)[population[i].second].size(), j);

							(*dna)[population[i].second][j] = mutatedDna;
						}
					}
				}
				//crossover possibilities
				//get some code and then switch some lines
				//
				//params:
				//	crossoverPercent:       how liekly crossover is to happen
				//	crossoverLengthMax:     maximum lines to get switched
				//	crossoverLengthMin:     minimum lines to get switched
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
			}

			cpu::OpArgument<cpu_t> getRandomLeft(string op) {
				OpArgCombos opArgsCombo = opArgs.at(removeSpace(op));
				vector<MemoryType>* possibleArgs = &opArgsCombo.left;

				if (possibleArgs->size() <= 0)return { NOTHING, 0 };

				MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());
				MemoryTypeRange range = (*cpu).bruhMap.count({ removeSpace(op),argType }) ? (*cpu).bruhMap.at({ removeSpace(op),argType }) : (*cpu).rangesMap.at(argType);

				return { argType, (cpu_t)drand(range.max - range.min + 1) + range.min };
			}

			cpu::MemoryType getRandomLeftType(string op) {
				OpArgCombos opArgsCombo = opArgs.at(removeSpace(op));
				vector<MemoryType>* possibleArgs = &opArgsCombo.left;

				if (possibleArgs->size() <= 0)return NOTHING;

				MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());

				return argType;
			}

			cpu::OpArgument<cpu_t> getRandomRight(string op, cpu::MemoryType left) {
				OpArgCombos opArgsCombo = opArgs.at(removeSpace(op));
				vector<MemoryType>* possibleArgs = &opArgsCombo.right;

				if (possibleArgs->size() <= 0)return { NOTHING, 0 };

				MemoryType argType = *selector(possibleArgs->begin(), possibleArgs->end());
				MemoryTypeRange range = (*cpu).bruhMap2.count({ removeSpace(op),left,argType }) ? (*cpu).bruhMap2.at({ removeSpace(op),left,argType }) : (*cpu).rangesMap.at(argType);

				return { argType, (cpu_t)drand(range.max - range.min + 1) + range.min };
			}

			cpu::MemoryType getRandomRightType(string op, cpu::MemoryType left) {
				printf("op: '%s'\n", removeSpace(op.c_str()).c_str());
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
				(*cpu).resetRegisters();
				(*cpu).resetMemory();
				for (int i = 0; i < inputs->size(); i++) {
					(*cpu).setMemory(i, inputs->at(i));
				}
			}
			void applyDna(dna_t dna) {//the code
				string code;
				for (int i = 0; i < dna.size(); i++) {
					string str = dna[i];
					code.append(str);
				}
				//printf("%s\n", code.c_str());
				cpu->resetRegisters();
				progLength = (*cpu).assemble(&program->at(0), code.c_str(), code.size());
				cpu->resetRegisters();
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