#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <map>

typedef std::map<std::string, std::pair<std::vector<std::string>, int> > Tree;

static const int INF = (1 << 30);
static const std::string alphabet = "qwertyuiopasdfghjklzxcvbnm0123456789";

std::string getWord(std::string str, size_t& ind)
{
	size_t first = ind;

	while (ind < str.size() && str[ind] >= 'a' && str[ind] <= 'z')
		++ ind;

	return str.substr(first, ind - first);
}

int getNumber(std::string str, size_t& ind)
{
	int nr = 0;

	while (ind < str.size() && str[ind] >= '0' && str[ind] <= '9') {
		nr = nr * 10 + str[ind] - '0';
		++ ind;
	}

	return nr;
}

std::string searchRoot(std::map<std::string, int> freq)
{
	for (const auto& x : freq)
		if (x.second == 1) 
			return x.first;
}

bool checkIfBalanced(std::vector<int> V)
{
	for (const auto& x : V)
		if (x != V[0])
			return false;

	return true;
}

int majority(std::vector<int> V)
{
	int k = 0, candidate;

	for (const auto& x: V) {
		if (k == 0)
			candidate = x;
		
		if (candidate == x)
			++ k;
		else
			-- k;
	}

	return candidate;
}

int unbalancedTree(Tree& arb, std::string root, std::string& unbNode)
{
	if (arb[root].first.empty())
		return arb[root].second;

	std::vector<int> V;;

	for (const auto& x : arb[root].first) {
		 V.push_back(unbalancedTree(arb, x, unbNode));

		if (!unbNode.empty())
			return -1;
	}

	if (!checkIfBalanced(V)) {
		int majEl = majority(V);

		for (size_t i = 0; i < V.size(); ++ i)
			if (V[i] != majEl) {
				arb[arb[root].first[i]].second += majEl - V[i];
				unbNode = arb[root].first[i];

				return -1;
			}
	}

	return arb[root].second + (int)V.size() * V[0];
}

int main()
{
	std::ifstream fin("data.in");
	std::string line;
	Tree arb;
	std::map<std::string, int> freq;

	while(std::getline(fin, line)) {
		size_t ind = line.find_first_of(alphabet);
		std::string procName = getWord(line, ind);

		if (freq.find(procName) == freq.end())
			freq[procName] = 0;

		++ freq[procName];
		ind = line.find_first_of(alphabet, ind + 1);
		
		int procWeight = getNumber(line, ind);
		std::vector<std::string> W;

		ind = line.find_first_of(alphabet, ind + 1);

		while (ind != std::string::npos) {
			std::string word = getWord(line, ind);

			if (freq.find(word) == freq.end())
				freq[word] = 0;

			++ freq[word];
			W.push_back(word);
			ind = line.find_first_of(alphabet, ind + 1);
		}

		arb[procName] = std::make_pair(W, procWeight);
	}

	std::string root = searchRoot(freq);
	std::string unbNode;

	unbalancedTree(arb, root, unbNode);

	std::cout << "The answer for part1 is: " << root << "\n";
	std::cout << "The answer for part2 is: " << arb[unbNode].second << "\n";;

	fin.close();

	return 0;
}