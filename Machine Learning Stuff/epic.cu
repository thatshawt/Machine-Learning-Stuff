#pragma once
#define T 1024

#include "epic.h"
#include "SpikingNeurons.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void epic::neuronKernel(SpikingNeuron* neurons, int length) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < length) {
		SpikingNeuron neuron = neurons[i];
		if (neuron.v >= 30) {
			neuron.v = neuron.params.c;
			neuron.u += neuron.params.d;
			//TODO: implement the I thing
		}
		neuron.v += (0.04 * (neuron.v * neuron.v) + 5 * (double)neuron.v + 140 - neuron.u + neuron.I);
		neuron.u += neuron.params.a * (neuron.params.b * neuron.v - neuron.u);
	}

}

void epic::doNeuronRat(SpikingNeuron* neurons, int length) {
	cudaMallocManaged((void**)neurons, sizeof(SpikingNeuron) * length);

	neuronKernel <<<(int)ceil(length / T), T >>> (neurons, length);
	cudaDeviceSynchronize();

	cudaFree(&neurons);
}