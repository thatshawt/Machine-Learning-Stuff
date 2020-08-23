#pragma once

#include <vector>
#include <ctime>

struct NeuronParams {
	/*******************/
	/*   parameters	      |range:(min <-> max) |*/
	/**/float a = 0.02;/* |      0.02 <-> 0.1  |*/
	/**/float b = 0.2; /* |		  0.2 <-> 0.25 |*/
	/**/float c = -64; /* |		  -65 <-> -60  |*/
	/**/float d = 2;   /* |		    2 <-> 8    |*/
	/*******************/

	/***************************/
	/*    some technicality    */
	/**/bool inhibit = false;/**/
	/***************************/

	static NeuronParams random();
};

struct SpikingNeuron {
	/******************/
	/*   variables    */
	/**/float v = 0;/**/
	/**/float u = 0;/**/
	/******************/

	//letsss GOOOOOOOO
	NeuronParams params;

	/******************/
	/*     input      */
	/**/float I = 0;/**/
	/******************/

	SpikingNeuron();

	SpikingNeuron(NeuronParams params);
};

struct MaskConfiguration {
	int start;// this is always defined
	//int length;//this is going to be -1 for the last phase signifying that it lasts forever

	std::vector<std::vector<char>> stdpMask;
	std::vector<std::vector<char>> weightMask;

	static MaskConfiguration getDefault(int neurons);
};

extern class SNN {
private:
	std::vector<MaskConfiguration> maskConfigs;

	std::vector<std::vector<int>> preSynapticTiming;
	std::vector<std::vector<int>> postSynapticTiming;

	std::vector<std::vector<float>> weights;
	std::vector<SpikingNeuron> neurons;

	unsigned long long t = 0;
public:
	SNN(int neurons, std::vector<NeuronParams> neuronConfigs);
protected:
	float aPlus = 1;
	float aMinus = 1;
	float tauPlus = 20;
	float tauMinus = 20;
public:
	void addMaskConfig(MaskConfiguration phase);

	void tickWithSTDP(unsigned long long milliseconds, bool debug, float(*getInput)(int));

	void tickImproved(long milliseconds);

	int pruneWeakConnections(int phase, float differenceThreshold, bool debug);

	int pruneWeakConnectionsImproved(int phase, int threshold);

	SpikingNeuron getNeuron(int i);

	int getActiveConnections(int phase);

	void printNeuron(SpikingNeuron* neuron);

	void printNeurons();

	void doNeuronRat(SpikingNeuron* neurons, int length);

};