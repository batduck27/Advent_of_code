#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "knoth.h"

std::vector<int> getNumbers(std::string str)
{
	std::vector<int> V;

	for (size_t i = 0; i < str.size(); ++ i) {
		if (str[i] >= '0' && str[i] <= '9') {
			int nr = 0;

			while (i < str.size() && str[i] >= '0' && str[i] <= '9') {
				nr = nr * 10 + str[i] - '0';
				++ i;
			}

			V.push_back(nr);
		}
	}
	
	return V;
}

int solveTask1(std::string buffer)
{
	std::vector<int> L = getNumbers(buffer);
	int v[DIM];
	int skipSize = 0;
	size_t ind = 0;

	for (int i = 0; i < DIM; ++ i) {
		v[i] = i;
	}

	KnothRound(L, v, skipSize, ind);

	return v[0] * v[1];
}

std::string solveTask2(std::string buffer)
{
	return KnothHash(buffer);
}

int main()
{
	std::ifstream fin("data.in");
	std::string buffer;

	std::getline(fin, buffer);
	fin.close();

	std::cout << "The answer for part1 is: " << solveTask1(buffer) << "\n";
	std::cout << "The answer for part2 is: " << solveTask2(buffer) << "\n";

	return 0;
}