#include <cxxtest/TestSuite.h>

#define MEMORY_SIZE 10
#define REGISTER_SIZE 5
#include <CpuThreading.h>

class MyTests : public CxxTest::TestSuite{
public:

    void testCpuRegisterPostCheck(){
        cpu::CpuSimulator<int> cpu;
        unsigned char program[0xFFF];

        std::string nothingProgram = "\nmov r2 69";

        int progLength = cpu.assemble(&program[0], &nothingProgram[0], nothingProgram.length());

        cpu.runProgram(&program[0], progLength, false);//false cus doReset = false

        int r2 = cpu.getRegister(2);

        TS_ASSERT(r2 == 69);
    }

    void testCpuRegisterPreSet(){
        cpu::CpuSimulator<int> cpu;
        unsigned char program[0xFFF];

        std::string nothingProgram = "\nmov r2 69";

        int progLength = cpu.assemble(&program[0], &nothingProgram[0], nothingProgram.length());

        cpu.setRegister(1, 420);

        cpu.runProgram(&program[0], progLength, false);//false cus doReset = false

        int r1 = cpu.getRegister(1);

        TS_ASSERT(r1 == 420);
    }

// "\nmov f2 1\nmov f3 1\nmov ptr 0\nmov mem 420\nmov r1 69";
    void testCpuMemoryPostSetViaProgram(){
        cpu::CpuSimulator<int> cpu;
        unsigned char program[0xFFF];

        std::string specialRat = "\nmov f2 1\nmov f3 1\nmov ptr 0\nmov mem 420";

        int progLength = cpu.assemble(&program[0], &specialRat[0], specialRat.length());

        cpu.runProgram(&program[0], progLength, false);//false cus doReset = false

        int m0 = cpu.getMemory(0);

        TS_ASSERT(m0 == 420);
    }

    void testCpuMemoryPreSet(){
        cpu::CpuSimulator<int> cpu;
        unsigned char program[0xFFF];

        std::string specialRat = "\nmov f2 1\nmov f3 1\nmov ptr 0\nmov mem 420";

        int progLength = cpu.assemble(&program[0], &specialRat[0], specialRat.length());

        cpu.setMemory(2, 69);

        cpu.runProgram(&program[0], progLength, false);//false cus doReset = false

        int m2 = cpu.getMemory(2);

        TS_ASSERT(m2 == 69);
    }

    void testCpuThreadingTookJustRight(){
        cpu::CpuSimulator<int> cpu;
		cpu::CpuRunProgramThread<int> cpuThread(&cpu);

        unsigned char program[0xFFF];

        std::string specialRat = "\nmov f2 1\nmov f3 1\nmov ptr 0\nmov r0 mem\nmov r1 69";

        int progLength = cpu.assemble(&program[0], &specialRat[0], specialRat.length());

        cpuThread.runProgram(&program[0], progLength, false);//false cus doReset = false
        const bool tookTooLong = cpuThread.sleepUntilComplete(5);

        TS_ASSERT(tookTooLong == false);
    }

    void testCpuThreadingTookTooLong(){
        cpu::CpuSimulator<int> cpu;
		cpu::CpuRunProgramThread<int> cpuThread(&cpu);

        unsigned char program[0xFFF];

        std::string specialRat = "\nmov r0 1\njmp 0";

        int progLength = cpu.assemble(&program[0], &specialRat[0], specialRat.length());

        cpuThread.runProgram(&program[0], progLength, false);//false cus doReset = false
        const bool tookTooLong = cpuThread.sleepUntilComplete(5);

        TS_ASSERT(tookTooLong == true);
    }

};