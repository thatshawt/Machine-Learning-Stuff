#define MEMORY_SIZE 5

#include "Cpu2.h"
#include <chrono>
//#include "helper.h"

int main(void) {
	typedef unsigned char byte;

	cpu::initMaps();

	cpu::CpuSimulator<int> simulator;
	simulator.verbose = true;
	simulator.step = false;

	byte prog[0xFFFF];
	int progLength = simulator.assemble(prog, "bruh.txt");

	printf("assembled\n");

	bool doTime = false;
	if (doTime) {
		Timer timer;
		long long counter = 0;
		int times = 10000;
		for (int i = 0; i < times; i++) {
			timer.start();
			simulator.runProgram(prog, progLength);
			counter += timer.getNano();
		}
		printf("Average Execution time: %d ns\n", (int)counter / times);
	}
	else {
		simulator.runProgram(prog, progLength);
	}
	simulator.dumpRegisters();
	printf("\n");
	simulator.dumpFlags();
	printf("\n");
	simulator.dumpPointers();
	printf("\n");
	simulator.dumpMemory();
	printf("\n");

	getchar();

	return 1;
}