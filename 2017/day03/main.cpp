#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

static const int NMAX = 1000;
static const int vert[] = {-1, -1, -1, 0, 1, 1, 1, 0};
static const int oriz[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int getManhattan(int n, int x)
{
	int posCenter = (n / 2) + 1;
	int nr = (n - 2) * (n - 2) + 1;

	for (int i = n - 1; i; nr ++, -- i)
		if (nr == x)
			return (n - posCenter + abs(i - posCenter));

	for (int j = n - 1; j; nr ++, -- j)
		if (nr == x)
			return (posCenter - 1 + abs(j - posCenter));

	for (int i = 2; i <= n; nr ++, ++ i)
		if (nr == x)
			return (posCenter - 1 + abs(i - posCenter));

	for (int j = 2; j <= n; nr ++, ++ j)
		if (nr == x)
			return (n - posCenter + abs(j - posCenter));

	return 0;
}

int getNeighboursSum(int mat[][NMAX], int i, int j)
{
	int sum = 0;

	for (int k = 0; k < 8; ++ k)
		sum += mat[i + vert[k]][j + oriz[k]];

	return sum;
}

int getLargerValue(int n, int x)
{
	int mat[NMAX][NMAX] = {0};
	int posCenter = (n / 2) + 1;
	int auxNG = posCenter + 1;
	int auxNL = posCenter - 1;

	mat[posCenter][posCenter] = 1;

	while (1) {
		for (int i = auxNG - 1; i >= auxNL; -- i) {
			mat[i][auxNG] = getNeighboursSum(mat, i, auxNG);

			if (mat[i][auxNG] > x)
				return mat[i][auxNG];
		}

		for (int j = auxNG - 1; j >= auxNL; -- j) {
			mat[auxNL][j] = getNeighboursSum(mat, auxNL, j);

			if (mat[auxNL][j] > x)
				return mat[auxNL][j];
		}

		for (int i = auxNL + 1; i <= auxNG; ++ i) {
			mat[i][auxNL] = getNeighboursSum(mat, i, auxNL);

			if (mat[i][auxNL] > x)
				return mat[i][auxNL];
		}

		for (int j = auxNL + 1; j <= auxNG; ++ j) {
			mat[auxNG][j] = getNeighboursSum(mat, auxNG, j);

			if (mat[auxNG][j] > x)
				return mat[auxNG][j];
		}

		auxNG ++;
		auxNL --;
	}

	return 1;
}

int main()
{	
	int x, n, y;
	std::ifstream fin("data.in");

	fin >> x;

	for (n = 1; n * n < x; n += 2);

	std::cout << "The answer for part1 is: " << getManhattan(n, x) << "\n";
	std::cout << "The answer for part2 is: " << getLargerValue(n, x) << "\n";

	fin.close();

	return 0;
}