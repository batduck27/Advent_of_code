#include <iostream>
#include <fstream>
#include <map>
#include <vector>

std::vector<std::string> V;

int solvePart1() {
	int doubles = 0, triples = 0;
	std::map<char, int> M;

	for (const auto& str : V) {
		int d = 0, t = 0;

		for (size_t i = 0; i < str.size(); ++ i) {
			M[str[i]] ++;
		}

		for (size_t i = 0; i < str.size(); ++ i) {
			switch (M[str[i]]) {
				case 2:
					d = 1;
					break;
				case 3:
					t = 1;
					break;
				default:
					break;
			}
		}

		doubles += d;
		triples += t;

		M.clear();
	}

	return doubles * triples;
}

std::pair<int, int> checkDiff(const std::string& str1, const std::string& str2) {
	int diffs = 0;
	int pos = 0;

	for (size_t i = 0; i < str1.size(); ++ i) {
		if (str1[i] != str2[i]) {
			diffs ++;
			pos = (int)i;
		}
	}

	return std::pair<int, int> {diffs, pos};
}

std::string solvePart2() {

	for (size_t i = 0; i < V.size(); ++ i) {
		for (size_t j = i + 1; j < V.size(); ++ j) {
			std::pair<int, int> tmp = checkDiff(V[i], V[j]);

			if (tmp.first == 1) {
				return V[i].substr(0, tmp.second) + V[i].substr(tmp.second + 1);
			}
		}
	}

	return "[Error] There is nothing...";
}

int main() {
	std::ifstream fin("data.in");
	std::string str;

	while (fin >> str) {
		V.push_back(str);
	}

	std::cout << "The answer for part1 is: " << solvePart1() << "\n";
	std::cout << "The answer for part2 is: " << solvePart2() << "\n";
	
	fin.close();

	return 0;
}