#include <iostream>
#include <fstream>
#include <vector>
#include <map>

const std::map<std::string, int> AuntSue {
	{"children", 3},
	{"cats", 7},
	{"samoyeds", 2},
	{"pomeranians", 3},
	{"akitas", 0},
	{"vizslas", 0},
	{"goldfish", 5},
	{"trees", 3},
	{"cars", 2},
	{"perfumes", 1}
};

const int NUMBER_POS = 1;

std::vector<std::string> tokenize(const std::string str) {
    static const std::string delim = " .:,";
    std::vector<std::string> words;
    size_t start = 0, end = 0;

    while (start != std::string::npos) {
        end = str.find_first_of(delim, start);
        words.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delim, end);
    }

    return words;
}

int solvePart1(const std::vector<std::vector<std::string>>& aunts) {
	int auntNumber;

	for (const auto& a : aunts) {
		bool flag = true;

		for (size_t i = NUMBER_POS + 1; i < a.size(); i += 2) {
			if (AuntSue.find(a[i])->second != std::stoi(a[i + 1])) {
				flag = false;
				break;
			}
		}

		if (flag) {
			auntNumber = std::stoi(a[NUMBER_POS]);
			break;
		}
	}

	return auntNumber;
}

int solvePart2(const std::vector<std::vector<std::string>>& aunts) {
	int auntNumber;

	for (const auto& a : aunts) {
		bool flag = true;

		for (size_t i = NUMBER_POS + 1; i < a.size(); i += 2) {
			std::map<std::string, int>::const_iterator it = AuntSue.find(a[i]);

			if (a[i] == "cats" || a[i] == "trees") {
				if (std::stoi(a[i + 1]) <= it->second) {
					flag = false;
					break;
				}
			} else if (a[i] == "pomeranians" || a[i] == "goldfish") {
				if (std::stoi(a[i + 1]) >= it->second) {
					flag = false;
					break;
				}
			} else if (std::stoi(a[i + 1]) != it->second) {
				flag = false;
				break;
			}
		}

		if (flag) {
			auntNumber = std::stoi(a[NUMBER_POS]);
			break;
		}
	}

	return auntNumber;
}

int main() {
	std::ifstream fin("data.in");
	std::string str;
	std::vector<std::vector<std::string>> aunts;

	while (std::getline(fin, str)) {
		aunts.push_back(tokenize(str));
	}

	std::cout << "The answer for part1 is: " << solvePart1(aunts) << "\n";
	std::cout << "The answer for part2 is: " << solvePart2(aunts) << "\n";

	fin.close();

	return 0;
}