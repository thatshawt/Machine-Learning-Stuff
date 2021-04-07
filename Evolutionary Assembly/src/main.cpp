//#include <windows.h>

#define MEMORY_SIZE 1 //idk we just want some ez strats you feel?

//#include <helper.h>
//#include "Cpu2.h"
#include "Genetic.h"
//#include "CpuThreading.cpp"

//#include <termcolor.hpp>

using namespace gene;

vector<int> identityFuncX(int x){
	vector<int> result;
	result.push_back(x);
	return result;
}

vector<int> identityFuncY(vector<int> x){
	vector<int> result(REGISTER_SIZE, 0);
	for(int i=0; i<x.size(); i++){
		result.at(i) = x[i];
	}
	return result;
}

vector<int> xorWith2FuncY(vector<int> x){
	vector<int> result(REGISTER_SIZE, 0);
	for(int i=0; i<x.size(); i++){
		result.at(i) = x[i] ^ 2;
	}
	return result;
}

vector<int> squareIdentityFuncY(vector<int> x){
	vector<int> result(REGISTER_SIZE, 0);
	for(int i=0; i<x.size(); i++){
		result.at(i) = (int)x[i] * (int)x[i];
	}
	return result;
}

int main() {
	////printf("%s%s\n", termcolor::red, "one time i bruhed so hard that my cat deid");
	//printf("shannon entropy of 'abc': %f\n", shannonEntropy("abc"));

	//printf("drand(yuj): %f\n", frand(10));

	cpu::CpuSimulator<int> cpuz;
	GeneticAssemblyRatz<int> genz(&cpuz);

	// {
	// 	//---------IDK WE JUST RUN A PROGRAM HERE---------
	// 	printf("PROGRAM EXECUTION TEST\n");
		
	// 	CpuRunProgramThread<int> cpuThread(&cpuz);
	// 	unsigned char prog[0xFFFF];
	// 	int progLength = 0;
	// 	//cpuz.verbose = true
	// 	cpuz.verbose = true;
	// 	progLength = cpuz.assemble(prog, "evolution.txt");
	// 	cpuz.verbose = false;
	// 	//cpuz.runProgram(prog, progLength);
	// 	//printf("running program\n");
	// 	printf("progLength: %d\n", progLength);

	// 	cpuz.setMemory(0, 69);//simple test to see if setMemory does what it needs to do

	// 	cpuThread.runProgram(&prog[0], progLength);
	// 	bool completed = cpuThread.sleepUntilComplete(10);
	// 	printf("%s\n", completed ? "finished execution" : "execution took too long");

	// 	cpuz.dumpRegisters();
	// 	cpuz.dumpPointers();
	// 	cpuz.dumpMemory();
	// 	//---------END---------
	// }

	{
		//---------GENE TEST---------
		printf("EVOLUTION TEST\n");
		//TODO: do a test where we see if these parameters should be evolved
		int generations = 10000000; //TODO: add an option to make the generations last until we reach a threshold fitness
		int species             = 20;
		genz.params.tournamentK = 10;
		// cpuz.verbose = true;
		// genz.step = true;
		
		genz.params.newDnaChance = 0.95f;
		
		genz.params.maxProgTime = 5; //5 milliseconds i guess mah boi

		genz.params.mutatePercent = 0.7f;
		genz.params.mutateNewOpChance = 0.058f;
		genz.params.mutateDeleteOpChance = 0.055f;
		genz.params.mutateOpThreshold = 0.01f;
		genz.params.mutateRange = 2;

		printf("nomimasu ka?\n");
		vector<vector<int>> x = gene::generateX(&identityFuncX, 0, 10);
		// vector<vector<int>> y = gene::generateY(&xorWith2FuncY, x);
		vector<vector<int>> y = gene::generateY(&squareIdentityFuncY, x);
		// vector<vector<int>> y = gene::generateY(&identityFuncY, x);

		for(int i = 0;i < x.size(); i++){
			for(int j = 0; j < x[i].size(); j++){
				printf("x(%d,%d): %d\n", i, j, x[i][j]);
			}
		}

		for(int i = 0;i < y.size(); i++){
			for(int j = 0; j < y[i].size(); j++){
				printf("y(%d,%d): %d\n", i, j, y[i][j]);
			}
		}

		// cpuz.verbose = true;

		auto before = std::chrono::system_clock::now();
		genz.train(generations, species, x, y);
		auto after = std::chrono::system_clock::now();

		auto duration = after - before;
		auto seconds = std::chrono::duration_cast<chrono::seconds>(duration);

		printf("elapsed minutes: %f\n", ((float)seconds.count())/60.0f);
		//---------END---------
	}
	//Sleep(3000);

	// getchar();
	
	return 0;
}

/*
this solves the identity problem somehow

jgt 89 
mov ptr mem
shl mem r5
and r5 15
or mem -22

mov r0 mem


*/


