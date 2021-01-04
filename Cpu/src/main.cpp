#define MEMORY_SIZE 5

// #include "Cpu2.h"
#include "CpuThreading.h"
#include <chrono>
//#include "helper.h"

int main(void) {
	typedef unsigned char byte;

	cpu::initMaps();

	cpu::CpuSimulator<int> simulator;
	cpu::CpuRunProgramThread<int> cpuThread(&simulator);
	// simulator.verbose = true;
	// simulator.step = true;

	byte prog[0xFFFF];
	int progLength = simulator.assemble(prog, "bruh.txt");

	//check for a lot of inputs or something
	for(int i=0;i<100;i++){
		simulator.setMemory(0, i);
		cpuThread.runProgram(prog, progLength);
		cpuThread.sleepUntilComplete(5);//wait 5 millis max
		printf("(x,y): (%d, %d)\n", i, simulator.getRegister(0));
	}

	getchar();

	return 1;
}