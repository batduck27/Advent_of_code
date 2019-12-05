#include <iostream>
#include <fstream>
#include <vector>

#define NMAX 2000

int getNumber(std::string str, size_t& ind)
{
	int nr = 0;

	for(; ind < str.size() && str[ind] >= '0' && str[ind] <= '9'; ++ ind)
		nr = nr * 10 + str[ind] - '0';

	return nr;
}

std::vector<int> tokenizer(std::string str)
{
	const std::string alphabet = "0123456789";
	std::vector<int> V;
	size_t ind = str.find_first_of(alphabet);

	while (ind != std::string::npos) {
		V.push_back(getNumber(str, ind));
		ind = str.find_first_of(alphabet, ind + 1);
	}

	return V;
}

int DFS(const std::vector<int> G[], int vis[], int node)
{
	if (vis[node])
		return 0;

	vis[node] = 1;

	for (const auto& x: G[node])
		if (!vis[x])
			vis[node] += DFS(G, vis, x);

	return vis[node];
}

int main()
{
	std::ifstream fin("data.in");
	std::vector<int> G[NMAX];
	std::string line;
	int nr;

	while (fin >> nr) {
		std::getline(fin, line);
		G[nr] = tokenizer(line);
	}

	fin.close();

	int vis[NMAX] = {0};
	int groupsCounter = 0;

	for (int i = 0; i < NMAX; ++ i)
		if (!vis[i]) {
			DFS(G, vis, i);
			++ groupsCounter;
		}

	std::cout << "The answer for part1 is: " << vis[0] << "\n";
	std::cout << "The answer for part2 is: " << groupsCounter << "\n";
	
	return 0;
}