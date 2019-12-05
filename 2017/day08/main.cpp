#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>

static const std::string incStr = "inc";

bool checkCondition(std::string expr, int op1, int op2)
{
	if (expr == ">")
		return op1 > op2;

	if (expr == "<")
		return op1 < op2;

	if (expr == "==")
		return op1 == op2;

	if (expr == "!=")
		return op1 != op2;

	if (expr == ">=")
		return op1 >= op2;

	if (expr == "<=")
		return op1 <= op2;

	return false;
}

std::vector<std::string> tokenizer(std::string str)
{
	std::vector<std::string> W;

	for (size_t last = 0, curr = 0; curr < str.size(); ++ curr)
		if (str[curr] != ' ') {
			last = curr;

			while (curr < str.size() && str[curr] != ' ')
				++ curr;

			W.push_back(str.substr(last, curr - last));
		}

	return W;
}

int stringToNumber(std::string str)
{
	int nr = 0;
	int negative = 1;
	size_t ind = 0;

	if (str[ind] == '-') {
		negative = -1;
		ind ++;
	}

	for (; ind < str.size(); ++ ind)
		nr = nr * 10 + str[ind] - '0';

	return negative * nr;
}

int doInstruction(std::string instr, int op1, int op2)
{
	if (instr == incStr)
		return op1 + op2;
	
	return op1 - op2;
}

int main()
{
	std::ifstream fin("data.in");
	std::string line;
	std::map<std::string, int> M;
	int finalMax = 0;
	int interMax = 0;

	while (std::getline(fin, line)) {
		std::vector<std::string> W = tokenizer(line);

		if (M.find(W[0]) == M.end())
			M.insert(std::make_pair(W[0], 0));

		if (M.find(W[4]) == M.end())
			M.insert(std::make_pair(W[4], 0));

		if (checkCondition(W[5], M[W[4]], stringToNumber(W[6]))) {
			M[W[0]] = doInstruction(W[1], M[W[0]], stringToNumber(W[2]));
			interMax = std::max(interMax, M[W[0]]);
		}
	}

	for (const auto& x : M)
		finalMax = std::max(finalMax, x.second);

	std::cout << "The answer for part1 is: " << finalMax << "\n";
	std::cout << "The answer for part2 is: " << interMax << "\n";

	fin.close();	

	return 0;
}