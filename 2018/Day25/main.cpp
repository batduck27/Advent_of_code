#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <map>

#include <stdio.h>
#include <stdlib.h>

typedef std::tuple<int, int, int, int> Point;
static const int MAX_DIST = 3;

int Manhattan(const Point& i, const Point& j) {
	auto [a1, b1, c1, d1] = i;
	auto [a2, b2, c2, d2] = j;

	return abs(a1 - a2) + abs(b1 - b2) + abs(c1 - c2) + abs(d1 - d2);
}

bool find(const std::vector<Point>& constellation, const Point& x) {
	for (const auto& y : constellation) {
		if (Manhattan(x, y) <= MAX_DIST) {
			return true;
		}
	}

	return false;
}

int main() {
	std::ifstream fin("data.in");
	std::string str;
	std::vector<Point> V;

	while (std::getline(fin, str)) {
		int a, b, c, d;

		sscanf(str.c_str(), "%d,%d,%d,%d", &a, &b, &c, &d);
		V.push_back(Point(a, b, c, d));
	}

	std::vector<int> dad(V.size()), rang(V.size());
	std::map<int, std::vector<Point>> pad;

	for (size_t i = 0; i < V.size(); ++ i) {
		std::vector<int> tmp;

		for (const auto& x : pad) {
			if (find(x.second, V[i])) {
				tmp.push_back(x.first);
			}
		}

		if (tmp.size() == 0) {
			pad[i] = std::vector<Point>(1, V[i]);
		} else {
			pad[tmp[0]].push_back(V[i]);

			for (size_t j = 1; j < tmp.size(); ++ j) {
				pad[tmp[0]].insert(pad[tmp[0]].begin(), pad[tmp[j]].begin(), pad[tmp[j]].end());
				pad.erase(tmp[j]);
			}
		}
	}

	std::cout << "The answer for part1 is: " << pad.size() << "\n";
	std::cout << "MERRY CHRISTMAS! Congratz!" << "\n";

	fin.close();

	return 0;
}