#include <iostream>
#include <fstream>
#include <vector>	
#include <map>
#include <utility>
#include <algorithm>

typedef std::vector<std::string> GRID;
typedef std::map<GRID, GRID > MAP;

static const int ITERATIONS = 18;

GRID rowSplit(std::string mat)
{
	GRID aux;
	size_t last = 0, curr = 0;

	while (curr < mat.size()) {
		curr = mat.find_first_of("/", last);
		aux.push_back(mat.substr(last, curr - last));
		last = curr + 1;
	}

	return aux;
}

void rotateMatrix(GRID& mat)
{
	for (size_t i = 0; i < mat.size(); ++ i)
		for (size_t j = i; j < mat[i].size(); ++ j)
			std::swap(mat[i][j], mat[j][i]);

	for (size_t i = 0; i < mat.size(); ++ i)
		for (size_t j = 0; j < mat[i].size() / 2; ++ j)
			std::swap(mat[i][j], mat[i][mat.size() - j - 1]);
}

GRID flipOriz(const GRID& mat)
{
	GRID aux(mat.size());

	for (int i = 0; i < mat.size(); ++ i) {
		aux[i] = mat[i];
		std::reverse(aux[i].begin(), aux[i].end());
	}

	return aux;
}

GRID flipVert(const GRID& mat)
{
	GRID aux(mat.size());

	for (int i = 0; i < mat.size(); ++ i)
		aux[i] = mat[mat.size() - i - 1];

	return aux;
}

void getRotations(MAP& M, GRID mat, GRID res)
{
	for (int i = 0; i < 4; ++ i) {
		M[mat] = M[flipOriz(mat)] = M[flipVert(mat)] = res;
		rotateMatrix(mat);
	}
}

GRID cropMatrix(const GRID& mat, int i1, int i2, int j1, int j2)
{
	GRID aux;

	for (int i = i1; i < i2; ++ i)
		aux.push_back(mat[i].substr(j1, j2 - j1));

	return aux;
}

GRID iteration(const GRID& mat, MAP M)
{
	int dim = 3;

	if (mat.size() % 2 == 0)
		dim = 2;

	GRID aux(mat.size() + mat.size() / dim);

	for (int i = 0; i < mat.size() / dim; ++ i)
		for (int j = 0; j < mat.size() / dim; ++ j) {
			GRID tmp = M[cropMatrix(mat, i * dim, (i + 1) * dim, j * dim, (j + 1) * dim)];
		
			for (int k = 0; k < tmp.size(); ++ k)
				aux[i * tmp.size() + k].append(tmp[k]);
		}

	return aux;
}

int countPixelsOn(const GRID& mat)
{
	int pixelsOn = 0;

	for (const auto& x : mat)
		for (const auto& i : x)
			if (i == '#')
				++ pixelsOn;

	return pixelsOn;
}

int main()
{
	std::ifstream fin("data.in");
	MAP M;
	std::string config, junk, res;

	while (fin >> config) {
		fin >> junk >> res;
		getRotations(M, rowSplit(config), rowSplit(res));
	}

	fin.close();

	GRID mat = rowSplit(".#./..#/###");

	for (int i = 0; i < ITERATIONS; ++ i) {
		if (i == 5)
			std::cout << "The answer for part1 is: " << countPixelsOn(mat) << "\n";

		mat = iteration(mat, M);
	}

	std::cout << "The answer for part2 is: " << countPixelsOn(mat) << "\n";

	return 0;
}