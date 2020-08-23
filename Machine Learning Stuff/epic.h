#pragma once

#include "SpikingNeurons.h"

namespace epic {
	void __global__ neuronKernel(SpikingNeuron* neurons, int length);

	void doNeuronRat(SpikingNeuron* neurons, int length);
}