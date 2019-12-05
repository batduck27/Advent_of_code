#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "knoth.h"

static const int GRIDSIZE = 128;
static const int vert[] = {-1, 0, 1, 0};
static const int oriz[] = {0, 1, 0, -1};

int HextoDec(char ch)
{
	if (ch >= 'a' && ch <= 'f')
		return (int)(ch - 'a' + 10);

	return (int)(ch - '0');
}

void computeGrid(std::string input, int grid[][GRIDSIZE])
{
	for (int i = 0; i < GRIDSIZE; ++ i) {
		std::string hash = KnothHash(input + "-" + std::to_string(i));

		for (int j = 0; j < hash.size(); ++ j) {
			int nr = HextoDec(hash[j]);
			int e = 3;

			while (e >= 0) {
				grid[i][4 * j + e] = nr % 2;
				nr /= 2;
				-- e;
			}
		}
	}
}

void fillRegion(int grid[][GRIDSIZE], const int i, const int j)
{	
	grid[i][j] = 0;

	for (int k = 0; k < 4; ++ k) {
		int newI = i + vert[k];
		int newJ = j + oriz[k];

		if (newI < 0 || newJ < 0 || newI >= GRIDSIZE || newJ >= GRIDSIZE)
			continue;

		if (grid[newI][newJ] == 1)
			fillRegion(grid, newI, newJ);
	}
}

int main()
{
	std::ifstream fin("data.in");
	std::string input;
	int grid[GRIDSIZE][GRIDSIZE] ={0};

	fin >> input;
	fin.close();
	computeGrid(input, grid);

	int bitCounter = 0;
	int regionCounter = 0;

	for (int i = 0; i < GRIDSIZE; ++ i)
		for (int j = 0; j < GRIDSIZE; ++ j)
			bitCounter += grid[i][j];

	for (int i = 0; i < GRIDSIZE; ++ i)
		for (int j = 0; j < GRIDSIZE; ++ j)
			if (grid[i][j] == 1) {
				fillRegion(grid, i, j);
				++ regionCounter;
			}

	std::cout << "The answer for part1 is: " << bitCounter << "\n";
	std::cout << "The answer for part2 is: " << regionCounter << "\n";

	return 0;
}
