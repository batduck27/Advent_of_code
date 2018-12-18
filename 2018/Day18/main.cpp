#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define OPEN   (char)('.')
#define TREE   (char)('|')
#define LUMBER (char)('#')

const int PART1_ITER = 10;
const int PART2_ITER = 1000000000;
const int N = 50;
const int vert[] = {-1, -1, -1, 0, 1, 1, 1, 0};
const int oriz[] = {-1, 0, 1, 1, 1, 0, -1, -1};

char countNeighbours(const std::vector<std::string>& grid, int y, int x) {
	int open = 0, tree = 0, lumber = 0;

	for (int dir = 0; dir < 8; ++ dir) {
		int newY = y + vert[dir];
		int newX = x + oriz[dir];

		if (newY >= 0 && newX >= 0 && newY < N && newX < N) {
			switch (grid[newY][newX]) {
				case OPEN:
					open ++;
					break;
				case TREE:
					tree ++;
					break;
				case LUMBER:
					lumber ++;
					break;
				default:
					break;
			}
		}
	}

	char ret = grid[y][x];

	if (grid[y][x] == OPEN && tree >= 3) {
		ret = TREE;
	} else if (grid[y][x] == TREE && lumber >= 3) {
		ret = LUMBER;
	} else if (grid[y][x] == LUMBER && (lumber < 1 || tree < 1)) {
		ret = OPEN;
	} else {
		// do nothing
	}

	return ret;
}

int getResourceValue(const std::vector<std::string>& grid) {
	int lumber = 0, tree = 0;

	for (int i = 0; i < N; ++ i) {
		for (int j = 0; j < N; ++ j) {
			lumber += (grid[i][j] == LUMBER);
			tree += (grid[i][j] == TREE);
		}
	}

	return lumber * tree;
}

int run(std::vector<std::string> grid, int iterations) {
	for (int it = 0; it < iterations; ++ it) {
		std::vector<std::string> tmp(grid);

		for (int i = 0; i < N; ++ i) {
			for (int j = 0; j < N; ++ j) {
				tmp[i][j] = countNeighbours(grid, i, j);
			}
		}

		grid = tmp;
	}

	int lumber = 0, tree = 0;

	for (int i = 0; i < N; ++ i) {
		for (int j = 0; j < N; ++ j) {
			lumber += (grid[i][j] == LUMBER);
			tree += (grid[i][j] == TREE);
		}
	}

	return lumber * tree;
}

int main() {
	std::ifstream fin("data.in");
	std::string str;
	std::vector<std::string> grid;

	while (std::getline(fin, str)) {
		grid.push_back(str);
	}

	std::vector<std::vector<std::string>> M;
	std::vector<std::vector<std::string>>::iterator it;

	while ((it = std::find(M.begin(), M.end(), grid)) == M.end()) {
		M.push_back(grid);

		std::vector<std::string> tmp(grid);

		for (int i = 0; i < N; ++ i) {
			for (int j = 0; j < N; ++ j) {
				tmp[i][j] = countNeighbours(grid, i, j);
			}
		}

		grid = tmp;
	}

	int cycle_pos = it - M.begin();
	int part2_pos = (PART2_ITER - cycle_pos) % (M.size() - cycle_pos) + cycle_pos;

	std::cout << "The answer for part1 is: " << getResourceValue(M[PART1_ITER]) << "\n";
	std::cout << "The answer for part2 is: " << getResourceValue(M[part2_pos]) << "\n";

	fin.close();

	return 0;
}