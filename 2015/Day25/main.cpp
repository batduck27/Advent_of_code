#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdio.h>

const long long FIRST = 20151125;
const long long FACTOR = 252533;
const long long MOD = 33554393;
const std::string frmt = "To continue, please consult the code grid in the manual.  Enter the code at row %d, column %d.";

int main() {
	std::ifstream fin("data.in");
	std::string str;
	int row, column;

	std::getline(fin, str);
	sscanf(str.c_str(), frmt.c_str(), &row, &column);

	int n = row + column - 1;
	long long el = FIRST;

	for (int i = 1; i < n * (n - 1) / 2 + column; ++ i) {
		el = el * FACTOR % MOD;
	}

	std::cout << "The answer for part1 is: " << el << "\n";

	fin.close();

	return 0;
}