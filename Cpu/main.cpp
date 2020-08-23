#include "Cpu.cpp"
#include <chrono>
#include "helper.h"

int main(void) {
	cpu::initMaps();

	cpu::CpuSimulator<int> simulator;
	simulator.verbose = true;
	simulator.step = true;

	byte prog[0xFFFF];
	int progLength = simulator.assemble(prog, "bruh.txt");

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
		printf("Average Execution time: %d ns\n", counter / times);
	}
	else {
		simulator.runProgram(prog, progLength);
	}
	simulator.dumpRegisters();
	printf("\n");
	simulator.dumpFlags();
	printf("\n");
	simulator.dumpPointers();

	getchar();

	return 1;
}