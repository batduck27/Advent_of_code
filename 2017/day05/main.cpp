#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void read(std::istream& fin, std::vector<int>& V)
{
	int tmp;

	while (fin >> tmp)
		V.push_back(tmp);
}

int countSteps(std::vector<int> V, bool isTask2)
{
	int ind = 0;
	int steps = 0;

	while (ind >= 0 && ind < V.size()) {
		int indCopy = ind;

		ind += V[indCopy];

		if (isTask2 == true && V[indCopy] > 2)
			V[indCopy] --;
		else
			V[indCopy] ++;

		steps ++;
	}

	return steps;
}

int main()
{
	std::ifstream fin("data.in");
	std::vector<int> V;

	read(fin, V);
	
	std::cout << "The answer for part1 is: " << countSteps(V, false) << "\n";
	std::cout << "The answer for part2 is: " << countSteps(V, true) << "\n";

	fin.close();

	return 0;
}