#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

static const int vert[] = {1, 1, 0, -1, 0, -1};
static const int oriz[] = {1, 0, 1, -1, -1, 0};

typedef struct Point {
	int x;
	int y;
} Point;

std::string getWord(std::string str, size_t& ind)
{
	size_t first = ind;

	while (ind < str.size() && str[ind] >= 'a' && str[ind] <= 'z')
		++ ind;

	return str.substr(first, ind - first);
}

std::vector<std::string> tokenizer(std::string str)
{
	const std::string alphabet = "nwes";
	std::vector<std::string> W;
	size_t ind = str.find_first_of(alphabet);

	while (ind != std::string::npos) {
		W.push_back(getWord(str, ind));
		ind = str.find_first_of(alphabet, ind + 1);
	}

	return W;
}

int direction(std::string step)
{
	if (step == "n")
		return 0;

	if (step == "ne")
		return 1;

	if (step == "nw")
		return 2;

	if (step == "s")
		return 3;

	if (step == "se")
		return 4;

	if (step == "sw")
		return 5;
}

// https://stackoverflow.com/a/15516953
int computeDistance(const Point& A, const Point& B)
{
	Point distance;
	distance.x = A.x - B.x;
	distance.y = A.y - B.y;

	Point diagonalMovement;
	Point straightMovement;
	int lesserCoord = abs(distance.x) < abs(distance.y) ? abs(distance.x) : abs(distance.y);
	
	diagonalMovement.x = (distance.x < 0) ? -lesserCoord : lesserCoord; 
	diagonalMovement.y = (distance.y < 0) ? -lesserCoord : lesserCoord;

	straightMovement.x = distance.x - diagonalMovement.x;
	straightMovement.y = distance.y - diagonalMovement.y;

	size_t straightDistance = abs(straightMovement.x) + abs(straightMovement.y);
	size_t diagonalDistance = abs(diagonalMovement.x);

	if ( (diagonalMovement.x < 0 && diagonalMovement.y > 0) || 
		 (diagonalMovement.x > 0 && diagonalMovement.y < 0) )
    	diagonalDistance *= 2;

	return straightDistance + diagonalDistance;
}

int main()
{
	std::ifstream fin("data.in");
	std::string buffer;

	std::getline(fin, buffer);
	fin.close();

	std::vector<std::string> W = tokenizer(buffer);

	Point A = {0, 0};
	int maxDistance = 0;

	for (const auto& step : W) {
		int ind = direction(step);

		A.x += vert[ind];
		A.y += oriz[ind];

		maxDistance = std::max(maxDistance, computeDistance({0, 0}, A));
	}

	std::cout << "The answer for part1 is: " << computeDistance({0, 0}, A) << "\n";
	std::cout << "The answer for part2 is: " << maxDistance << "\n";

	return 0;
}