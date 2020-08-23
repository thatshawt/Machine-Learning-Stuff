#include <vector>

class Perceptron {
private:
	std::vector<float> inputs;
	std::vector<float> weights;

	std::vector<float> activationInputs;
};

class Activation {
private:
	std::vector<float> vars;
public:
	virtual std::vector<float> activation(std::vector<float> vars) = 0;
};