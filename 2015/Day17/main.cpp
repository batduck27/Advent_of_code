#include <iostream>
#include <fstream>
#include <vector>

#define TARGET (int)(150)

std::vector<int> V;
int combinations;
int minContainerNr = (1 << 30);
int minContainerCnt = 0;

void back(int ind = 0, int sum = 0, int containersUsed = 0) {
	if (sum == TARGET) {
		++ combinations;

		if (minContainerNr > containersUsed) {
			minContainerNr = containersUsed;
			minContainerCnt = 1;
		} else if (minContainerNr == containersUsed) {
			minContainerCnt ++;
		}

		return;
	}

	for (size_t i = ind; i < V.size(); ++ i) {
		if (sum + V[i] <= TARGET) {
			back(i + 1, sum + V[i], containersUsed + 1);
		}
	}
}

int main() {
	std::ifstream fin("data.in");
	int nr;

	while (fin >> nr) {
		V.push_back(nr);
	}

	back();

	std::cout << "The answer for part1 is: " << combinations << "\n";
	std::cout << "The answer for part2 is: " << minContainerCnt << "\n";

	fin.close();

	return 0;
}
