#include <string>
#include <vector>

namespace genes{

    struct gene{
        int innovationNumber;
        std::string data;
    };

    struct genome{
        std::vector<gene> genes;
    }

    bool geneSortCompare(gene gene1, gene gene2){
        return gene1.innovationNumber > gene2.innovationNumber;
    }

    //gonna add speciation lesss gooo
    float geneDelta(genome genome1, genome genome2, float c1, float c2){
        //disjoint, excess
        std::vector<gene> genes1 = std::vector(genome1.genes);
        std::vector<gene> genes2 = std::vector(genome2.genes);
        std::sort(genes1.begin(), genes1.end(), geneSortCompare);//test to see if this is backwards or something
        std::sort(genes2.begin(), genes2.end(), geneSortCompare);

        std::vector<gene>* bigger = genes1.size() >= genes2.size() ? &genes1 : &genes2;
        std::vector<gene>* smaller = genes1.size() <= genes2.size() ? &genes1 : &genes2;

        float disjoint,excess=0.0f;
        
        //we can caluclate excess genes by subtracting the lengths ez
        // excess = bigger->sizer() - smaller->size();

        //we can calculate the disjoint genes also by doing a difference of innovation numbers i guess
        int smalli = 0;//the actual index
        int bigi = 0;
        while(smalli < smaller->size()){
            int small = smaller->at(smalli); //innovation number
            int big = smaller->at(bigi);
            if(small == big){
                smalli++;
                bigi++;
            }else if(small < big){
                smalli++;

                disjoint++;
            }else if(small > big){
                bigi++;

                disjoint++;
            }
        }

        excess = bigger->size() - smalli;

        float N = bigger->size() < 20 ? 1 : (float)bigger->size();

        return (c1*excess)/N + (c2*disjoint)/N; //idk what to do with the term (c3*W)
        //in the original neat paper, W = average weight differences of matching genes
        //W seems to be related to the data inside the genes
        //so it would seem obvious to make W related to the vm instruction in the gene or something
    }

    //the sharing function
    float sh(genome genome1, genome genome2, float deltaThreshold, float c1, float c2){
        //they are too different to be considered the same species
        if(geneDelta(genome1, genome2, c1, c2) >= deltaThreshold){
            return 0.0f;
        }else{ //they are same species i guess
            return 1.0f;
        }
    }

    //TODO: turn this into real code
    //adjusted fitness function
    float fPrime(float fitness, genome genomeA, genome[] population, float deltaThreshold, float c1, float c2){
        float sum = 0.0f;
        for(int i=0;i<population.size();i++){
            sum += sh(geneDelta(genomeA, population[i], c1, c2), deltaThreshold, c1, c2);
        }
        return fitness/sum;
    }

    void neat(){
        std::vector<genome> population;
        std::vector<std::vector<genome*>> species;

        //innovation numbers
        //  instructionHash, innovationNumber
        std::hashmap<int, int> innovationNumbers;

    }

};