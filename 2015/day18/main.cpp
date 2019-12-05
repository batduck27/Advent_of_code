#include <iostream>
#include <fstream>
#include <vector>

#define N   (int)(100)
#define ON  (char)('#')
#define OFF (char)('.')

const int vert[] = {-1, -1, -1, 0, 1, 1, 1, 0};
const int oriz[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int neighboursOn(const std::vector<std::string>& v, int y, int x) {
	int ret = 0;

	for (int k = 0; k < 8; ++ k) {
		int newX = x + oriz[k];
		int newY = y + vert[k];

		if (newX >= 0 && newY >= 0 && newX < N && newY < N &&
			v[(size_t)newY][(size_t)newX] == ON) {
			++ ret;
		}
	}

	return ret;
}

int tick(std::vector<std::string>& grid, bool part2 = false) {
	std::vector<std::string> tmp(grid);
	int lightsOn = 0;

	for (size_t i = 0; i < N; ++ i) {
		for (size_t j = 0; j < N; ++ j) {
			int neighsOn = neighboursOn(grid, i, j);

			if (part2 && (i == 0 || i == N - 1) && (j == 0 || j == N - 1)) {
				lightsOn ++;
				continue;
			}

			if (neighsOn == 3 || (grid[i][j] == ON && neighsOn == 2)) {
				tmp[i][j] = ON;
				lightsOn ++;
			} else {
				tmp[i][j] = OFF;
			}
		}
	}

	grid = tmp;

	return lightsOn;
}

int run(std::vector<std::string> grid, int iterations, bool part2 = false) {
	int lightsOn;

	if (part2) {
		grid[0][0] = grid[N - 1][N - 1] = grid[N - 1][0] = grid[0][N - 1] = ON;
	}

	for (int i = 0; i < iterations; ++ i) {
		lightsOn = tick(grid, part2);
	}

	return lightsOn;
}

int main() {
	std::ifstream fin("data.in");
	std::string str;
	std::vector<std::string> grid;

	while (std::getline(fin, str)) {
		grid.push_back(str);
	}

	std::cout << "The answer for part1 is: " << run(grid, 100) << "\n";
	std::cout << "The answer for part2 is: " << run(grid, 100, true) << "\n";

	fin.close();

	return 0;
}
