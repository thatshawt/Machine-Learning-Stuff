#include <vector>
#include <map>
#include "helper.h"

typedef std::vector<bool> cell;

namespace ca {
	class CA2d {
	private:
		//we need two cus a technicalityyty
		std::vector<cell> cellsA;
		std::vector<cell> cellsB;
		bool A;//just some switch variable

		//map<neighbors, resultingCell>
		std::map<std::vector<cell>, cell> rule;

		//we can leave this running for a LOOOONGG time lollLLLL
		unsigned long long timeStep;

		long width, height;

	public:
		CA2d(long width, long height, std::map<std::vector<cell>, cell> rule) {
			this->width = width;
			this->height = height;

			this->rule = rule;
		}

		//forgot which one this is but game of life uses it so it must be good lol
		//this gets the 9 cells which includes the center cell
		std::vector<cell> getNeighborhood(std::vector<cell> neighborhood, long x, long y) {
			neighborhood.clear();
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					neighborhood.push_back(getCell(x + i, y + j, !A));
				}
			}
			return neighborhood;
		}

		//lol is just a switch
		cell getCell(long x, long y, bool lol) {
			std::vector<cell>* currentCells =
				(std::vector<cell>*)((lol * ((unsigned)&cellsA))
					+ (lol * ((unsigned)&cellsB)));
			///the thing above achieves the same thing as the commented block below.
			//i think the top version is faster cus i didnt use if statements which apparently
			//makes things way slower.
			/*
			if (A) {
				currentCells = &cellsA;
			}
			else {
				currentCells = &cellsB;
			}
			*/
			return currentCells->at(y * height + x);
		}

		//oooo daayyuuummm
		void step() {
			A = !A;
			std::vector<cell> neighborhood;
			for (long x = 0; x < width; x++) {
				for (long y = 0; y < height; y++) {
					cell currentCell = getCell(x, y, A);
					cell result = rule.at(getNeighborhood(neighborhood, x, y));

					currentCell = cell(result);//copy cell into the thing it do
				}
			}
			timeStep++;
		}
	};

	//dimension is used to determine the cells per neighborhood and valuesPerCell is self explanatory
	std::map<std::vector<cell>, cell> randomRule(int dimension, int valuesPerCell) {
		int cellsPerNeigborhood = pow(3,dimension);
	}



}