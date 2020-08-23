#define std STDS
#include <vector>
#include <string>
#include "Cpu.cpp"

using namespace STDS;//bruh ew get those stds out of here

namespace gene {

	struct costInfo {
		int species;
		double cost;
	};

	template <class dna_t>
	class GeneticAlgorithm {
	protected:
		double lowestCost;
		
		dna_t bestDna;

		int maxSpecies;

		virtual int getResult(vector<int> inputs) = 0;
		virtual void cost(double *currentCost, vector<int> expected, vector<int> result) = 0;
		virtual void postIterateCost(double *currentCost, vector<vector<int>> x) = 0;
		virtual double dnaDifference(dna_t dna1, dna_t dna2) = 0;
		virtual void geneticOperations(vector<dna_t> dna, costInfo costs[]) = 0;
		virtual void applyInput(vector<int> inputs) = 0;
		virtual void applyDna(dna_t dna) = 0;
		virtual dna_t getRandomDna() = 0;
	public:
		void train(int generations, int maxSpecies, vector<vector<int>> x, vector<vector<int>> y) {
			vector<dna_t> dna;
			
			for (int species = 0; species < maxSpecies; species++) {
				dna[species] = getRandomDna();
			}
			
			for (int gen = 0; gen < generations;gen++) {
				costInfo costs[maxSpecies];
				for (int species = 0; species < maxSpecies;species++) {
					double cost;
					for (int i = 0; i < x.size();i++ ) {
						applyInput(x[i]);
						applyDna(dnas[species]);

						auto expected = y[i];
						auto result = getResult(x[i]);

						cost(&cost, expected, result);
					}
					postIterateCost(&cost, x);
					costs[species].species = species;
					costs[species].cost = cost;
					totalCost += cost;
					printf("Species %d cost: %f\n", species, cost);

					if (cost <= 0.01) {
						printf("you win baws\n");
						for (int i = 0; i < x.size();i++) {
							vector<int> input = x[i];
							printf("x: ");
							arrayToString<vector<int>>(input, input.size());
							printf("y: %d\n", getResult(input));
						}
						return;
					}
				}
				sort(costs, costs[maxSpecies-1], compare);

				lowestCost = costs[0].cost;

				geneticOperations(dnas, costs);

				if (gen == generations-1) 
					bestDna = dnas[costs[0].species];
			}
		}

	private:
		bool compare(costInfo i, costInfo j) {
			return i.cost >= j.cost;
		}

		template <class type>
		static string arrayToString(type data, int length) {
			arrayToString<type>(data, length, ', ');
		}

		template <class type>
		static string arrayToString(type data, int length, char separator) {
			printf("{ ");
			for (int i = 0; i < length; i++) {
				printf("%s%c", to_string(data[i]), separator);
			}
			printf(" }\n");
		}
	};

	struct GeneticAssemblyParams {

	};

	typedef vector<string> dna_t;//yooooo letsss gooooo
	template <class cpu_t>
	class GeneticAssemblyRatz : public GeneticAlgorithm<dna_t>{
	public:
		GeneticAssemblyRatz(cpu::CpuSimulator<cpu_t> cpu) {
			this->cpu = cpu;
		}
		GeneticAssemblyParams params;
	private:
		const int maxProgLength = 0xFFFF;
		int progLength;
		byte program[maxProgLength];
		CpuSimulator<cpu_t> cpu;
		int getResult(vector<int> inputs) {
			applyInput(inputs);
			cpu.runProgram(program, progLength);
			return cpu.registers[0];
		}
		void cost(double* currentCost, vector<int> expected, vector<int> result) {
			double cost;
			for (int i = 0; i < result.size();i++) {
				cost += abs(expected[i]-result[i]);
			}
			*currentCost = cost;
		}
		void postIterateCost(double* currentCost, vector<vector<int>> x) {
			*currentCost = (*currentCost)/((double)x.size());
		}
		//not used in current version
		double dnaDifference(dna_t dna1, dna_t dna2) {

		}


		//population size basically
		int tournamentK = 5;
		float mutatePercent = 0.3;
		void geneticOperations(vector<dna_t> dna, costInfo costs[]) {
			//selection
			//im gonna use tournament selection cus i havent tried that yet
			//idk kinda cringe to implemet jk its ez mode lets go bois
			vector<dna_t> init = vector<dna_t>(dna);
			vector<dna_t> population;
			vector<int> indexes;
			int i = 0;
			while (population.size() < tournamentK) {
				//i think this is faster i have no idea lol
				i = ((i == init.size()) * 0) + ((i != init.size()) * i);
				//if (i == init.size())i = 0; this is the thing above

				//TODO: possible bug with stuff not having equal changes cus size always changing
				bool bruh = frand(1) <= (1/init.size());//each thing has equal chance sort of
				if (bruh && bruh2) {//then we choose it
					bool contains = find(indexes.begin(), indexes.end(), i) != indexes.end();
					if (!contains) {
						population.push_back(init[i]);
						indexes.push_back(i);
					}
				}
				i++;
			}//at this point we have our population of size tournamentK
			//so lets just pick the best one
			dna_t bestDna = max_element(population.begin(), population.end());

			//mutate possibilities
			//-change op into another instruction
			//-change one of params into one of the possibilites defined in some array idk
			//-delete an op
			//-create new op with random params
			//-do numerical operation on one of params
			//
			//params:
			//	mutatePercent:			how likely an opcode is to undergo mutation
			if (frand(1) <= mutatePercent) {

			}

			//crossover possibilities
			//get some code and then switch some lines
			//
			//params:
			//	crossoverPercent:       how liekly crossover is to happen
			//	crossoverLengthMax:     maximum lines to get switched
			//	crossoverLengthMin:     minimum lines to get switched
		}

		dna_t getRandomDna() {

		}
		void applyInput(vector<int> inputs) {//the registers? i think lol i forgot
			cpu.resetRegisters();
			cpu.resetMemory();
			for (int i = 0; i < inputs.size();i++) {
				cpu.registers[i] = inputs[i];
			}
		}
		void applyDna(dna_t dna) {//the code
			string code;
			for (int i = 0; i < dna.size();i++) {
				string str = dna[i];
				code.append(str);
			}
			progLength = assemble(program, code.c_str(), code.size() + 1);
		}

	};
}