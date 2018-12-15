#include <iostream>
#include <fstream>
#include <algorithm>

const int INF = (1 << 29);
const int N = 300;
const int NMAX = N + 5;

int SERIALKEY;
int mat[NMAX][NMAX];

void part1() {
	static const int size = 3;
	int maxSum = -INF;
	int indX, indY;

	for (int i = size; i <= N; ++ i) {
		for (int j = size; j <= N; ++ j) {
			int tmp = mat[i][j] - mat[i][j - size] - mat[i - size][j] + mat[i - size][j - size];

			if (tmp > maxSum) {
				maxSum = tmp;
				indY = j - size + 1;
				indX = i - size + 1;
			}
		}
	}

	std::cout << "The answer for part1 is: (" << indX << "," << indY << ")" << "\n";
}

void part2() {
	int maxSum = -INF;
	int indX, indY, size;

	for (int i = 1; i <= N; ++ i) {
		for (int j = 1; j <= N; ++ j) {
			for (int k = 1; k <= std::min(i, j); ++ k) {
				int tmp = mat[i][j] - mat[i][j - k] - mat[i - k][j] + mat[i - k][j - k];
			
				if (tmp > maxSum) {
					maxSum = tmp;
					size = k;
					indX = i - k + 1;
					indY = j - k + 1;
				}
			}
		}
	}

	std::cout << "The answer for part2 is: (" << indX << "," << indY  << "," << size << ")" << "\n";
}

int main() {
	std::ifstream fin("data.in");

	fin >> SERIALKEY;

	for (int i = 1; i <= N; ++ i) {
		for (int j = 1; j <= N; ++ j) {
			int rackID = i + 10;
			int aux = (((rackID * j + SERIALKEY) * rackID) % 1000) / 100 - 5;

			mat[i][j] = mat[i - 1][j] + mat[i][j - 1] - mat[i - 1][j - 1] + aux;
		}
	}

	part1();
	part2();

	fin.close();

	return 0;
}