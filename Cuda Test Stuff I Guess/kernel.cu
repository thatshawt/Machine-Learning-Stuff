#pragma once

//#include "cuda_runtime.h"
//#include <device_launch_parameters.h>

#include <stdio.h>
#include "../Machine Learning Stuff/SpikingNeurons.cpp"

//__global__ void firedNeuronKernel(SpikingNeuron* neurons, int length);
__global__ void firedNeuronKernel(SpikingNeuron* neurons, int length) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < length) {
        SpikingNeuron neuron = neurons[i];
        
    	//    v(fired)=c(fired);
		//    u(fired)=u(fired)+d(fired);
		//    I=I+sum(S(:,fired),2);
        
        if (neuron.v >= 30) {
            neuron.v = neuron.params.c;
            neuron.u += neuron.params.d;
            //TODO: implement the "I=I+sum(S(:,fired),2);"
        }
        neuron.v += (0.04 * (neuron.v * neuron.v) + 5 * (double)neuron.v + 140 - neuron.u + neuron.I);
        neuron.u += neuron.params.a * (neuron.params.b * neuron.v - neuron.u);
    }

}

#define T 1024
void doNeuronShit(SpikingNeuron* neurons, int length) {
    cudaMallocManaged(&neurons, sizeof(SpikingNeuron)*length);
    
    firedNeuronKernel <<<(int)ceil(length / T), T >>> (neurons, length);
    cudaDeviceSynchronize();

    //neuronKernel<<<(int)ceil(length / T), T >>> (neurons, length);
    //cudaDeviceSynchronize();
    
    cudaFree(&neurons);
}
