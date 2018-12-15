#include <iostream>
#include <fstream>
#include <set>

#include <stdio.h>

#define NMAX 1200
#define OVERLAP -1

int mat[NMAX][NMAX];
std::set<int> S;
int res;

void fill (int x, int y, int w, int h, int id) {
	std::set<int> toRemove;

	for (int i = x; i < x + w; ++ i) {
		for (int j = y; j < y + h; ++ j) {
			if (mat[i][j]) {
				if (mat[i][j] != OVERLAP) {
					toRemove.insert(mat[i][j]);
					res ++;
					mat[i][j] = OVERLAP;
				}

				toRemove.insert(id);
			} else {
				mat[i][j] = id;
			}
		}
	}

	for (const auto& x : toRemove) {
		S.erase(x);
	}
}

int main() {
	std::ifstream fin("data.in");
	std::string str;

	while (std::getline(fin, str)) {
		int id, x, y, w, h;

		sscanf(str.c_str(), "#%d @ %d,%d: %dx%d", &id, &x, &y, &w, &h);

		S.insert(id);

		fill(x, y, w, h, id);
	}

	std::cout << "The answer for part1 is: " << res << "\n";
	std::cout << "The answer for part2 is: " << *(S.begin()) << "\n";

	fin.close();

	return 0;
}