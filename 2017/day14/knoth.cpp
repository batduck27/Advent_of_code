#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "knoth.h"

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

std::string KnothHash(std::string input)
{
	std::vector<int> L = getASCIICodes(input);
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

