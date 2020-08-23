#include "SpikingNeurons.h";
#include "helper.h"

float inputHandler(int neuron) {
    switch(neuron){
    case 0://i just picked neuron zero cus why not
        return 1;
    default:
        return 0;
    }
}

float randomInputHandler(int neuron) {
    return 5 * frand(1);
}

/*
observations:
the more connections there are, the more pruning happens
the higher the tickTime the less the pruning

*/
int main() {
    Timer timer;
    int neurons = 10000;
    int tickTime = 1000;
    std::vector<NeuronParams> params;
    for (int i = 0; i < neurons;i++) {
        params.push_back(NeuronParams::random());
    }
    SNN brian = SNN(neurons, params);

    brian.addMaskConfig(MaskConfiguration::getDefault(neurons));

    int initConnections = brian.getActiveConnections(0);
    printf("\nStart with %d active connections\n", initConnections);

    int i = 0;
    int sum = 0;
    int zeros = 0;
    int pruned = 0;
    int pruneTime = 0;
    while (zeros < 10 //&& pruned < neurons
        ) {
        timer.start();
        //brian.tickWithSTDP(tickTime, false, randomInputHandler);
        brian.tickImproved(tickTime);
        sum += timer.getMilli();
        timer.start();
        //int temp = brian.pruneWeakConnections(0, 0.1f, false);
        int temp = brian.pruneWeakConnectionsImproved(0, 0.1f);
        pruneTime += timer.getMilli();
        pruned += temp;
        zeros += temp==0;

        printf("pruned: %d\n", temp);
        i++;
    }

    printf("run time: %dms, prune time: %dms\n", sum/i, pruneTime/i);
    printf("%d connections\n", brian.getActiveConnections(0));
    printf("%.1f percent pruned", (1-((double)brian.getActiveConnections(0) / (double)initConnections)) * 100);
	getchar();
}