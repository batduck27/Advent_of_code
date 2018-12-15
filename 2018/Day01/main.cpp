#include <iostream>
#include <fstream>
#include <vector>
#include <set>

std::vector<int> V;

int solvePart1() {
	int res = 0;

	for (const auto& x : V) {
		res += x;
	}

	return res;
}

int solvePart2() {
	int currFreq = 0;
	int ind = 0;
	std::set<int> S;

	for (size_t i = 0; i < V.size(); i = ((i == V.size() - 1) ? 0 : i + 1)) {
		if (S.find(currFreq) != S.end()) {
			break;
		}

		S.insert(currFreq);

		currFreq += V[i];
	}

	return currFreq;
}

int main() {
	std::ifstream fin("data.in");
	int nr;

	while (fin >> nr) {
		V.push_back(nr);
	}

	std::cout << "The result for part1 is: " << solvePart1() << "\n";
	std::cout << "The result for part2 is: " << solvePart2() << "\n";
	fin.close();

	return 0;
}