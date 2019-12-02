#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const long long INF = (1 << 30);

std::vector<int> V;
long long bestEntg = INF, bestSize = INF;

void back(int s, long long currEntg, int groupSize, int ind, const int& target) {
	if (s >= target) {
		if (s == target) {
			if (groupSize < bestSize) {
				bestSize = groupSize;
				bestEntg = currEntg;
			} else if (groupSize == bestSize && bestEntg > currEntg) {
				bestEntg = std::min(bestEntg, currEntg);
			} else {
				// do nothing
			}
		}
		return;
	}

	for (int i = ind; i < V.size(); ++ i) {
		back(s + V[i], currEntg * V[i], groupSize + 1, i + 1, target);
	}
}

int main() {
	std::ifstream fin("data.in");
	int nr, s = 0;

	while (fin >> nr) {
		V.push_back(nr);
		s += nr;
	}

	back(0, 1, 0, 0, s / 3);
	std::cout << "The answer for part1 is: " << bestEntg << "\n";
	back(0, 1, 0, 0, s / 4);
	std::cout << "The answer for part2 is: " << bestEntg << "\n";

	fin.close();

	return 0;
}