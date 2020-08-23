#pragma once

#include "SpikingNeurons.h"
#include "helper.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <thrust/device_vector.h>

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
#define gpuLog(ans) { gpuAssert((ans), __FILE__, __LINE__, false); }
inline void gpuAssert(cudaError_t code, const char* file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) {
			getchar();
			exit(code);
		}
	}
}

NeuronParams NeuronParams::random() {
	NeuronParams params;
	params.inhibit = !(frand(1) <= 0.8);

	if (params.inhibit) {
		//this->weights[j][i] = -frand(1);
		params.a = 0.02f + 0.08f * frand(1);
		params.b = 0.25f - 0.05f * frand(1);
		params.c = -65;
		params.d = 2;
	} else {
		//this->weights[j][i] = 0.5f * frand(1);
		params.a = 0.02f;
		params.b = 0.2f;
		params.c = -65 + 15 * pow(frand(1), 2);
		params.d = 8 - 6 * pow(frand(1), 2);
	}
	return params;
}


SpikingNeuron::SpikingNeuron() {

}

SpikingNeuron::SpikingNeuron(NeuronParams params) {
	this->params = params;

	this->v = -65;
	this->u = this->params.b * this->v;
}

MaskConfiguration MaskConfiguration::getDefault(int neurons) {
	MaskConfiguration config;
	config.start = 0;
	//config.length = -1;
	for (int i = 0; i < neurons; i++) {
		config.stdpMask.push_back(std::vector<char>(neurons, true));
		config.weightMask.push_back(std::vector<char>(neurons, true));
	}
	return config;
}



SNN::SNN(int neurons, std::vector<NeuronParams> neuronConfigs) {
	srand(std::time(0));

	this->neurons.reserve(neurons);
	weights.reserve(neurons);
	//weightMask.reserve(neurons);
	//stdpMask.reserve(neurons);
	preSynapticTiming.reserve(neurons);
	postSynapticTiming.reserve(neurons);
	for (int i = 0; i < neurons; i++) {
		//printf("i: %d\n",i);
		this->neurons.push_back(struct SpikingNeuron());
		weights.push_back(std::vector<float>(neurons,0));
		//weightMask.push_back(std::vector<bool>(neurons,true));
		//stdpMask.push_back(std::vector<bool>(neurons,true));
		preSynapticTiming.push_back(std::vector<int>(neurons,-1));
		postSynapticTiming.push_back(std::vector<int>(neurons, -1));
	}

	for (int i = 0; i < neurons;i++) {
		for (int j = 0; j < neurons; j++) {
			SpikingNeuron neuron = this->neurons[j];
			neuron = SpikingNeuron(neuronConfigs[j]);
				
			if (neuron.params.inhibit) {
				this->weights[j][i] = -frand(1);
			}
			else {
				this->weights[j][i] = 0.5f * frand(1);
			}
		}
	}
		
	for (int i = 0; i < neurons && false; i++) {
		//weights[i].reserve(neurons);
		//weightMask[i].reserve(neurons);
		preSynapticTiming[i].reserve(neurons);
		postSynapticTiming[i].reserve(neurons);
			
		//fill_n(weightMask[i], true);
		fill_n(postSynapticTiming[i], -1);
		fill_n(preSynapticTiming[i], -1);
	}
}
void SNN::addMaskConfig(MaskConfiguration phase) {
	this->maskConfigs.push_back(phase);
}

void SNN::tickWithSTDP(unsigned long long milliseconds, bool debug, float(*getInput)(int)) {
	int weightChanges = 0;
	int currentPhase = 0;
	unsigned long long prevT = t;
	for (;t-prevT < milliseconds; t++) {
		for (int j = 0; j < neurons.size(); j++) {
			//these are pointer to the masks we are gonna use
			std::vector<std::vector<char>>* stdpMask = &maskConfigs[currentPhase].stdpMask;
			std::vector<std::vector<char>>* weightMask = &maskConfigs[currentPhase].weightMask;

			SpikingNeuron *firingNeuron = &neurons[j];
			//firingNeuron->I = !firingNeuron->inhibit ? 5 * frand(1) : 2 * frand(1);//thalamic input
			firingNeuron->I = getInput(j);
			//printf("ratto: %f\n", firingNeuron->I);
			if (firingNeuron->v >= 30) {//it fired
				//printf("fired\n");
				//we need to do, timeDifference = timePost - timePre, then plug difference into the STDP learning equations

				firingNeuron->v = firingNeuron->params.c;
				firingNeuron->u += firingNeuron->params.d;

				// increment each I by the weights of the fired neurons
				// I = I + sum(S(:, fired);
				firingNeuron->I += 1;

				for (int i = 0; i < weights.size(); i++) {
					if ((*weightMask)[i][j])postSynapticTiming[i][j] = t;
				}
				for (int i = 0; i < weights[j].size(); i++) {//loops through the post neuron given the pre neuron
					if ((*weightMask)[j][i]) {
						preSynapticTiming[j][i] = t;

						SpikingNeuron *postSynaptic = &neurons[i];
						postSynaptic->I += weights[j][i];//send the signal to other neurons
						//check the timings to get the dt

						if ((*stdpMask)[j][i] && preSynapticTiming[j][i] != -1 && postSynapticTiming[j][i] != -1) {
							//courtesy of http://www.scholarpedia.org/article/Spike-timing_dependent_plasticity
							int dt = postSynapticTiming[j][i] - preSynapticTiming[j][i];
							float dw;
								
							if (dt > 0) {
								dw = (float)(aPlus * exp((-1 * dt) / tauPlus));//these are popular equations apparently
							}
							else {
								dw = (float)(-1 * aMinus * exp(dt / tauMinus));//this one as WELL
							}

							if (debug && dw > 0.01) printf("%d learn by this mushc: %f",j,dw);
							weights[j][i] += dw;
							preSynapticTiming[j][i] = -1;
							postSynapticTiming[j][i] = -1;
							weightChanges++;
						}
					}
				}
			}
			else {
				firingNeuron->v += (0.04 * pow(firingNeuron->v, 2) + 5 * (double)firingNeuron->v + 140 - firingNeuron->u + firingNeuron->I);
				//firingNeuron.v += 0.5*(0.04*Math.pow(firingNeuron.v,2) + 5*firingNeuron.v + 140 - firingNeuron.u + firingNeuron.I);
				//firingNeuron.v += 0.5*(0.04*Math.pow(firingNeuron.v,2) + 5*firingNeuron.v + 140 - firingNeuron.u + firingNeuron.I);
				firingNeuron->u += firingNeuron->params.a * (firingNeuron->params.b * firingNeuron->v - firingNeuron->u);
			}
			if (debug) printf("%d weight changes\n", weightChanges);
			for (int phase = 0; phase < maskConfigs.size();phase++) {
				if (maskConfigs[phase].start == t) {
					currentPhase = phase;
					break;
				}
			}
		}
	}
}

#define T 1024

__global__
void neuronKernel(SpikingNeuron* neurons, int length) {
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

void SNN::tickImproved(long milliseconds) {
	int weightChanges = 0;
	int currentPhase = 0;
	unsigned long long prevT = t;

	cudaMallocManaged((void**)&neurons[0], sizeof(SpikingNeuron) * neurons.size());
	cudaMallocManaged((void**)&postSynapticTiming[0], sizeof(postSynapticTiming) * postSynapticTiming.size());

	for (; t - prevT < milliseconds; t++) {
		std::vector<std::vector<char>>* stdpMask = &maskConfigs[currentPhase].stdpMask;
		std::vector<std::vector<char>>* weightMask = &maskConfigs[currentPhase].weightMask;

		neuronKernel <<<(int)ceil(neurons.size() / T), T >>> (&neurons[0], neurons.size());

		cudaDeviceSynchronize();

		//MaskConfig check
		for (int phase = 0; phase < maskConfigs.size(); phase++) {
			if (maskConfigs[phase].start == t) {
				currentPhase = phase;
				break;
			}
		}
	}

	cudaFree(&neurons);
}

int SNN::pruneWeakConnections(int phase, float differenceThreshold, bool debug) {
	int pruned = 0;
	for (int j = 0; j < neurons.size(); j++) {
		for (int i = 0; i < neurons.size(); i++) {
			if (maskConfigs[phase].weightMask[j][i] && abs(weights[j][i]) <= abs(differenceThreshold)) {
				maskConfigs[phase].weightMask[j][i] = false;
				pruned++;
			}
		}
	}
	if (debug) printf("%d pruned\n",pruned);
	return pruned;
}

__global__
void pruningKernel(int* weightRows, int* weightMaskRows,
					int col, int row, float threshold) {
	//printf("blockIdx: %d, blockDimx: %d, threadIdx: %d\n", blockIdx.x, blockDim.x, threadIdx.x);
	int i = (blockIdx.x * blockDim.x) + threadIdx.x;
	//printf("(%d)",i);
	if (i < col*row && i >= 0) {
		int x = (int)ceilf((float)(i/col));
		int y = i % col;

		printf("%d ", i); //TODO: this code block randomly gets executed what

		bool weightMask = *((int*)*(weightMaskRows + y) + x);
		float weight = *((int*)*(weightRows + y) + x);
		printf("weight: %0.2f, ", weight);//TODO: this isnt printing anything bruh
		if (weightMask && abs(weight) <= abs(threshold)) {
			*((int*)*(weightRows + y) + x) = false;
			printf("1");
		}
	}
}

int SNN::pruneWeakConnectionsImproved(int phase, int threshold){
	int before = getActiveConnections(phase);

	int* weightRows = new int[weights.size()];
	int* weightMaskRows = new int[maskConfigs[phase].weightMask.size()];

	for (int i = 0; i < weights.size();i++) {
		//printf("create row %d\n",i);
		//weightRows[i] = (float*)malloc(sizeof(float));
		weightRows[i] = (int)weights[i].data();
	}
	for (int i = 0; i < maskConfigs[phase].weightMask.size();i++) {
		//printf("create row %d\n", i);
		//weightMaskRows[i] = (char*)malloc(sizeof(char));
		weightMaskRows[i] = (int)maskConfigs[phase].weightMask[i].data();
	}

	printf("mask: %d, weights; %d, weightRows: %d, weightMaskRows: %d\n",
		sizeof(MaskConfiguration) * maskConfigs.size(),
		sizeof(float) * weights.size() * weights[0].size(),
		weights.size() * sizeof(float*),
		maskConfigs[phase].weightMask.size() * sizeof(char*));

	size_t free, total;
	gpuLog(cudaMemGetInfo(&free, &total));
	//printf("usage: %d, free: %d\n", total-free, free);

	//printf("active connetions: %d\n", getActiveConnections(phase));
	gpuErrchk(cudaMallocManaged((void**)weightRows, weights.size() * sizeof(int)));
	gpuLog(cudaMemGetInfo(&free, &total));
	//printf("usage: %d, free: %d\n", total - free, free);
	gpuErrchk(cudaMallocManaged((void**)weightMaskRows, maskConfigs[phase].weightMask.size() * sizeof(int)));
	gpuLog(cudaMemGetInfo(&free, &total));
	//printf("usage: %d, free: %d\n", total - free, free);

	//printf("active connetions: %d\n", getActiveConnections(phase));
	gpuErrchk(cudaMallocManaged((void**)&maskConfigs, sizeof(MaskConfiguration) * maskConfigs.size()));
	gpuLog(cudaMemGetInfo(&free, &total));
	//printf("usage: %d, free: %d\n", total - free, free);
	gpuErrchk(cudaMallocManaged((void**)&weights, sizeof(float) * weights.size()* weights[0].size()));
	gpuLog(cudaMemGetInfo(&free, &total));
	//printf("usage: %d, free: %d\n", total - free, free);

	//printf("active connetions: %d\n", getActiveConnections(phase));

	int blocks = (int)ceil((double)weights.size() / (double)T);
	//printf("blocks: %d\n",blocks);
	pruningKernel<<<blocks, T>>>(//&(maskConfigs[phase].weightMask),
														//&weights,
															weightRows, weightMaskRows,
															weights[0].size(), weights.size(), threshold);
	cudaDeviceSynchronize();

	printf("active connetions: %d\n",getActiveConnections(phase));

	cudaFree(&maskConfigs);
	cudaFree(&weights);
	cudaFree(weightRows);
	cudaFree(weightMaskRows);

	delete[] weightRows;
	delete[] weightMaskRows;

	printf("active connetions: %d\n", getActiveConnections(phase));

	return before - getActiveConnections(phase);
}

SpikingNeuron SNN::getNeuron(int i) {
	return neurons[i];
}

int SNN::getActiveConnections(int phase) {
	int count = 0;
	for (int i = 0; i < maskConfigs[phase].weightMask.size(); i++) {
		for (int j = 0; j < maskConfigs[phase].weightMask[i].size(); j++) {
			if (maskConfigs[phase].weightMask[i][j])count++;
		}
	}
	return count;
}

void SNN::printNeuron(SpikingNeuron *neuron) {
	printf("v:%.1f u:%.1f I:%.1f\n",neuron->v, neuron->u, neuron->I);
}

void SNN::printNeurons() {
	for (int i = 0; i < neurons.size();i++) {
		printf("%d ",i);
		printNeuron(&neurons[i]);
	}
}

void SNN::doNeuronRat(SpikingNeuron* neurons, int length) {
}