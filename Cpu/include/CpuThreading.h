#pragma once
#include "Cpu2.h"
#include <thread>
#include <mutex>

namespace cpu {
	CPU_TEMPLATE
	class CpuRunProgramThread{
	private:
		//Timer timer;
		CpuSimulator<cpu_t>** emulator;
		CpuSimulator<cpu_t>* temp;
		bool init = false;
		std::atomic<bool> isCompleteZ{ false };
		std::atomic<bool> isStarted{ false };
		std::atomic<bool> isDoneDying{ false };
		//std::atomic<bool> initComplete{ false };
		std::thread theThread;
		std::mutex mu;

		inline void subThreadRunThing(byte* code, int progLength, bool reset) {
			//printf("subThread id: %d\n", std::this_thread::get_id());
			//emulator->resetRegisters();
			//printf("actually run program\n");
			(*emulator)->runProgram(code, progLength, reset);
			isCompleteZ.store(true);
			//printf("thread finished\n");
			//killThread();
		}


	public:
		//in microseconds
		//std::atomic<int> sleepTime = {100};
		const int sleepTime = 500;

		//CPU_TEMPLATE
		inline CpuRunProgramThread(CpuSimulator<cpu_t>** cpuSim) {
			this->emulator = cpuSim;
			//printf("cpuSim: %p, emulator: %p, this: %p\n", cpuSim, emulator, this);
		}

		//CPU_TEMPLATE
		inline CpuRunProgramThread(CpuSimulator<cpu_t>* cpuSim) {
			this->temp = cpuSim;
			this->emulator = &temp;
			//printf("cpuSim: %p, emulator: %p, this: %p\n", cpuSim, emulator, this);
		}

		inline void runProgram(byte* code, int progLength, bool reset = false) {
			//printf("run program stasrt\n");
			// if(init)killThread(); //FIXME: init never gets set to false and the cpu always gets interrupted
			this->isCompleteZ.store(false);
			this->isStarted.store(false);
			this->isDoneDying.store(false);

			//mu.lock();
			//(*emulator)->resetInterrupt();
			//BUG: theThread is joinable and it errors
			this->theThread = std::thread(&CpuRunProgramThread<cpu_t>::subThreadRunThing, this, code, progLength, reset);
			this->theThread.detach();//now its running in the background
			//mu.unlock();
			//we want theThread to init before we roast it lol
			//timer.start();

			this->isStarted.store(true);

			//printf("function end\n");
			init = true;
		}

		inline void killThread() {
			// mu.lock();
			// if (theThread.joinable()) {
			// 	theThread.detach();
			// 	//printf("thread detatched from killThread()\n");
			// }
			//printf("killthread\n");
			isStarted.store(false);
			(*emulator)->interrupt();
			// mu.unlock();
		}

		inline bool isComplete() {
			return isCompleteZ.load();
		}

		inline bool sleepUntilComplete(long long maxMilli) {
			auto started = std::chrono::high_resolution_clock::now();
			auto endTime = std::chrono::milliseconds(maxMilli)
											+ started;

			// while (!isStarted.load() && !isComplete())std::this_thread::sleep_for(std::chrono::microseconds(10));

			bool tookTooLong = false;

			while (!isComplete()) {
				auto now = std::chrono::high_resolution_clock::now();
				if (now >= endTime) {
					killThread();
					tookTooLong = true;
				}
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}

			return tookTooLong;
		}

	};

}