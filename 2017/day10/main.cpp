#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

static const int DIM = 256;
static const int SUFFIX[] = {17, 31, 73, 47, 23};

std::vector<int> getNumbers(std::string str)
{
	std::vector<int> V;

	for (size_t i = 0; i < str.size(); ++ i) 
		if (str[i] >= '0' && str[i] <= '9') {
			int nr = 0;

			while (i < str.size() && str[i] >= '0' && str[i] <= '9') {
				nr = nr * 10 + str[i] - '0';
				++ i;
			}

			V.push_back(nr);
		}
	
	return V;
}

std::vector<int> getASCIICodes(std::string str)
{
	std::vector<int> V;

	for (const auto& x : str)
		V.push_back((int)x);

	for (int i = 0; i < 5; ++ i)
		V.push_back(SUFFIX[i]);

	return V;
}

void KnothRound(const std::vector<int>& L, int v[], int& skipSize, size_t& ind)
{
	for (const auto& length : L) {
		for (int i = 0; i < length / 2; ++ i)
			std::swap(v[(ind + i) % DIM], v[(ind +length - i - 1) % DIM]);

		ind = (ind + length + skipSize) % DIM;
		++ skipSize;
	}
}

int solveTask1(std::string buffer)
{
	std::vector<int> L = getNumbers(buffer);
	int v[DIM];
	int skipSize = 0;
	size_t ind = 0;

	for (int i = 0; i < DIM; ++ i)
		v[i] = i;

	KnothRound(L, v, skipSize, ind);

	return v[0] * v[1];
}

std::string solveTask2(std::string buffer)
{
	std::vector<int> L = getASCIICodes(buffer);
	int v[DIM];
	int skipSize = 0;
	size_t ind = 0;

	for (int i = 0; i < DIM; ++ i)
		v[i] = i;

	for (int i = 0; i < 64; ++ i)
		KnothRound(L, v, skipSize, ind);

	std::stringstream ss;

	for (int i = 0; i < 16; ++ i) {
		int tmp = 0;

		for (int j = 0; j < 16; ++ j)
			tmp ^= v[i * 16 + j];

		ss << std::setfill ('0') << std::setw(2) << std::hex << tmp;
	}

	return ss.str();
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