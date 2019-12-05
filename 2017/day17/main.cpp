#include <iostream>
#include <fstream>
#include <vector>

static const int COUNTER1 = 2017;
static const int COUNTER2 = 50000000;

int solveTask1(int steps)
{
	std::vector<int> V;
	int ind = 0;

	V.push_back(0);

	while (V.size() < COUNTER1) {
		ind = (ind + steps) % V.size() + 1;
		V.insert(V.begin() + ind, V.size());
	}

	ind = (ind + steps) % V.size() + 1;

	return V[ind];
}

int solveTask2(int steps)
{
	int ind = 0;
	int lastValue;
	
	for (int size = 1; size <= COUNTER2; ++ size) {
		ind = (ind + steps) % size + 1;

		if (ind == 1)
			lastValue = size;
	}

	return lastValue;
}

int main()
{
	std::ifstream fin("data.in");
	int steps;

	fin >> steps;
	fin.close();

	std::cout << "The answer for part1 is: " << solveTask1(steps) << "\n";
	std::cout << "The answer for part2 is: " << solveTask2(steps) << "\n";

	return 0;
}