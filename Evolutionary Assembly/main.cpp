//#include <windows.h>

#define MEMORY_SIZE 5 //idk we just want some ez strats you feel?

//#include <helper.h>
//#include "Cpu2.h"
#include "Genetic.h"
//#include "CpuThreading.cpp"

//#include <termcolor.hpp>

using namespace gene;

//#include <stdio.h>
int main() {
	////printf("%s%s\n", termcolor::red, "one time i bruhed so hard that my cat deid");
	//printf("shannon entropy of 'abc': %f\n", shannonEntropy("abc"));

	//printf("drand(yuj): %f\n", frand(10));

	cpu::CpuSimulator<int> cpuz;
	GeneticAssemblyRatz<int> genz(&cpuz);

	printf("some cool dna\n");
	for (int i = 0; i < 5;i++) {
		printf("%s",genz.getRandomSingleDna()[0].c_str());
	}
	printf("\n\n");

	{
		//---------TESTING MUTATION---------
		printf("test mutation start\n");
		std::vector<std::string> op = splitString(&string("cmp r3 0"), ' ');
		for (int i = 0; i < op.size(); i++) {
			printf("'%s'\n", op[i].c_str());
		}
		cpu::OpArgument<int> arg = cpu::string_to_arg<int>(op[1]);
		cpu::OpArgument<int> arg2 = cpu::string_to_arg<int>(op[2]);

		for (int i = 0; i < 5; i++) {
			genz.mutateOpArgValLeft(op[0], &arg);
			genz.mutateOpArgValRight(op[0], &arg, &arg2);
			printf("%s %s%d %s%d\n", op[0].c_str(), getPrefix(arg.type).c_str(), arg.val, getPrefix(arg2.type).c_str(), arg2.val);
		}
		printf("\n");
		//---------END---------
	}


	{
		//---------DNA DIFFERENCE TEST---------
		printf("test dna difference start\n");
		dna_t dna1 = { "jmp 1", "mov m0 r0" };
		dna_t dna2 = { "mov m0 m1" };
		printf("differnece: %f\n\n", genz.dnaDifference(dna1, dna2));
		//---------END---------
	}

	{
		//---------IDK WE JUST RUN A PROGRAM HERE---------
		printf("test run program start\n");
		CpuRunProgramThread<int> cpuThread(&cpuz);
		unsigned char prog[0xFFFF];
		int progLength = 0;
		//cpuz.verbose = true
		cpuz.verbose = true;
		progLength = cpuz.assemble(prog, "evolution.txt");
		cpuz.verbose = false;
		//cpuz.runProgram(prog, progLength);
		//printf("running program\n");
		printf("progLength: %d\n", progLength);
		cpuThread.runProgram(&prog[0], progLength);
		bool completed = cpuThread.sleepUntilComplete(10);
		printf("%s\n", completed ? "finished execution" : "execution took too long");
		//while (!cpuThread.isComplete()) {
		//	std::this_thread::sleep_for(std::chrono::nanoseconds(10));
		//	printf("slept 10 nanoseconds\n");
		//}
		
		cpuz.dumpRegisters();
		//---------END---------
	}

	{
		//---------GENE TEST---------
		printf("test gene start\n");
		int generations = 10000;
		int species = 20;
		genz.params.tournamentK = 20;
		vector<vector<int>> x = { {1},{2},{3} };
		vector<vector<int>> y = { {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
								  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
								  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };
		genz.train(generations, species, x, y);
		//---------END---------
	}
	//Sleep(3000);

	getchar();
	
	return 0;
}