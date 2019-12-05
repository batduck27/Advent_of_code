#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

static const int pX = 0;
static const int pY = 1;
static const int pZ = 2;
static const int vX = 3;
static const int vY = 4;
static const int vZ = 5;
static const int aX = 6;
static const int aY = 7;
static const int aZ = 8;
static const int PCOUNTER = 1000;
static const int TEST = 1000; // tho', it needs only 10 tests :)
static const std::string separators = ",><";

std::vector<int> parseLine(std::string line)
{
	std::vector<int> tmp;
	size_t last = 0;

	for (int i = 0; i < 3; ++ i) {
		last = line.find_first_of(separators, last + 1) + 1;

		for (int j = 0; j < 3; ++ j) {
			size_t curr = line.find_first_of(separators, last + 1);

			tmp.push_back(std::stoi(line.substr(last, curr - last )));
			last = curr + 1;
		}
	}

	return tmp;
}

void tick(std::vector<int> var[PCOUNTER], std::vector<int> ind)
{
	for (const auto& i : ind) {
		var[i][pX] += (var[i][vX] += var[i][aX]);
		var[i][pY] += (var[i][vY] += var[i][aY]);
		var[i][pZ] += (var[i][vZ] += var[i][aZ]);
	}
}

bool solveCollisions(std::vector<int> var[PCOUNTER], std::vector<int>& ind)
{
	std::map<std::vector<int>, std::vector<int> > M;
	bool collision = false;

	for (const auto& i : ind)
		M[std::vector<int>(var[i].begin(), var[i].begin() + 3)].push_back(i);

	for (const auto& x : M)
		if (x.second.size() > 1) {
			collision = true;

			for (const auto& p : x.second)
				ind.erase(std::find(ind.begin(), ind.end(), p));
		}

	return collision;
}

int solveTask1(std::vector<int> var[PCOUNTER], int partNr)
{
	int part = -1, min = (1 << 30);

	for (int i = 0; i < partNr; ++ i) {
		if (abs(var[i][aX]) + abs(var[i][aY]) + abs(var[i][aZ]) < min) {
			min = abs(var[i][aX]) + abs(var[i][aY]) + abs(var[i][aZ]);
			part = i;
		}
	}

	return part;	
}

int solveTask2(std::vector<int> var[PCOUNTER], int partNr)
{
	std::vector<int> ind;
	int noCollision = 0;

	for (int i = 0; i < partNr; ++ i)
		ind.push_back(i);

	while (noCollision < TEST) {
		tick(var, ind);

		if (solveCollisions(var, ind))
			noCollision = 0;
		else
			++ noCollision;
	}

	return ind.size();
}

int main()
{
	std::ifstream fin("data.in");
	std::string line;
	std::vector<int> var[PCOUNTER];
	int partNr = 0;

	while (std::getline(fin, line))
		var[partNr ++] = parseLine(line);	

	fin.close();

	std::cout << "The answer for part1 is: " << solveTask1(var, partNr) << "\n";
	std::cout << "The answer for part2 is: " << solveTask2(var, partNr) << "\n";

	return 0;
}
