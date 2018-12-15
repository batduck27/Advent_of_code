#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <deque>
#include <set>

#include <cctype>

#define SPACE " "

std::vector<std::string> V;
std::set<char> S;

std::string react(const std::string& str) {
	std::deque<char> D;

	for (const auto& x : str) {
		if (!D.empty() && (D.back() != x && (D.back() == toupper(x) || D.back() == tolower(x)))) {
			D.pop_back();
		} else {
			D.push_back(x);
		}
	}

	return std::string(D.begin(), D.end());
}

int solvePart1(const std::string& str) {
	return react(str).size();
}

int solvePart2(const std::string& str) {
	int min = str.size();

	for (const auto& x : str) {
		S.insert(tolower(x));
	}

	for (const auto& ch : S) {
		std::string newStr;

		for (const auto& x : str) {
			if (tolower(x) != ch) {
				newStr += x;
			}
		}
	
		min = std::min(min, solvePart1(newStr));
	}

	return min;
}

int main() {
	std::ifstream fin("data.in");
	std::string str;

	std::getline(fin, str);

	std::cout << "The answer for part1 is " << solvePart1(str) << "\n";
	std::cout << "The answer for part2 is " << solvePart2(str) << "\n";

	fin.close();

	return 0;
}