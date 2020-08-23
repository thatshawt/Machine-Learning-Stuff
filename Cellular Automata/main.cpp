#include "CA.cpp"
int main() {

	std::map<std::vector<cell>, cell> rule;
	rule.insert({ {{1,1,1}}, {1} });

	ca::CA2d ca = ca::CA2d(10,10,rule);

	const int steps = 10;
	for (int i = 0; i < steps;i++) {
		ca.step();
	}

	return 0;
}