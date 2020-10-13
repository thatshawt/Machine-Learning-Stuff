#pragma once
#include "Cpu2.H"
#include <thread>
#include <mutex>

//multithreading was fun but i dont think we need it rn
//maybe later we can use it to test programs faster or some poop like that
//but for now i only needed it to time the duration of a program
//we just need to have a timer going basically on the cpu and then thats it i guess lol

namespace cpu {
	CPU_T
	class CpuRunProgramThread{
	private:


		//Timer timer;
		CpuSimulator<cpu_t>** emulator;
		CpuSimulator<cpu_t>* temp;
		std::atomic<bool> isCompleteZ{ false };
		std::atomic<bool> isStarted{ false };
		std::atomic<bool> isDoneDying{ false };
		//std::atomic<bool> initComplete{ false };
		std::thread theThread;
		std::mutex mu;

		void subThreadRunThing(byte* code, int progLength) {
			//printf("subThread id: %d\n", std::this_thread::get_id());
			//emulator->resetRegisters();
			(*emulator)->runProgram(code, progLength);
			isCompleteZ.store(true);
			//printf("thread detatched by finishing\n");
			killThread();
		}


	public:
		//in microseconds
		std::atomic<int> sleepTime = {100};

		CPU_T
		CpuRunProgramThread(CpuSimulator<cpu_t>** cpuSim) {
			this->emulator = cpuSim;
			//printf("cpuSim: %p, emulator: %p, this: %p\n", cpuSim, emulator, this);
		}

		CPU_T
		CpuRunProgramThread(CpuSimulator<cpu_t>* cpuSim) {
			this->temp = cpuSim;
			this->emulator = &temp;
			//printf("cpuSim: %p, emulator: %p, this: %p\n", cpuSim, emulator, this);
		}

		void runProgram(byte* code, int progLength) {
			this->isCompleteZ.store(false);
			this->isStarted.store(false);
			this->isDoneDying.store(false);

			//mu.lock();
			this->theThread = std::thread(&CpuRunProgramThread<cpu_t>::subThreadRunThing, this, code, progLength);
			//mu.unlock();
			//we want theThread to init before we roast it lol
			//timer.start();

			this->isStarted.store(true);

			//printf("function end\n");
		}

		void killThread() {
			if (theThread.joinable()) {
				theThread.detach();
				//printf("thread detatched from killThread()\n");
			}
			(*emulator)->interrupt();
			this->isStarted.store(false);
		}

		bool isComplete() {
			return isCompleteZ.load();
		}

		//long long getMilli() {
		//	return timer.getMilli();
		//}

		bool sleepUntilComplete(long long maxMilli) {
			long long counter = 0;
			//printf("thread id from cpuThreading(main): %d\n", std::this_thread::get_id());
			//printf("emulator: %p, this: %p\n", emulator, this);
			(*emulator)->resetRegisters();
			while (!this->isComplete()) {
				if (!this->isStarted.load()) {
					std::this_thread::sleep_for(std::chrono::nanoseconds(100));
					continue;
				}
				//printf("slept\n");
				if (counter >= maxMilli*1000) {
					//theThread.detach();
					killThread();
					return false;
				}
				std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.load()));
				//TODO: maybe its faster if we cache the val at start of function idk man
				counter += 100;
			}
			return true;
		}

	};
}