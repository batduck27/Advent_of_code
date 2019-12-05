#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;	

static const int INSCOUNTER = 50;
static const int WAITTIME = 10000;

std::vector<std::string> stringSplit(std::string line)
{
	std::vector<std::string> V;

	for (size_t curr = 0, last = 0; curr < line.size(); ++ curr)
		if (line[curr] != ' ') {
			last = curr;

			while (curr < line.size() && line[curr] != ' ')
				++ curr;

			V.push_back(line.substr(last, curr - last));
		}

	return V;
}

long long getValue(std::map<std::string, long long>& M, std::string op)
{
	if (op[0] >= 'a' && op[0] <= 'h') {
		if (M.find(op) == M.end())
			M[op] = 0;

		return M[op];
	}

	return (long long)std::stoi(op);
}

int binaryOp(std::map<std::string, long long>& M, std::string ins, std::string op1, std::string op2)
{
	if (M.find(op1) == M.end())
		M[op1] = 0;

	if (ins == "set") {
		M[op1] = getValue(M, op2);
		return 1;
	}	

	if (ins == "sub") {
		M[op1] -= getValue(M, op2);
		return 1;
	}

	if (ins == "mul") {
		M[op1] *= getValue(M, op2);
		return 1;
	}

	if (ins == "jnz") {
		if (getValue(M, op1) != 0)
			return getValue(M, op2);

		return 1;
	}

	return 1;
}

int solveTask1(std::vector<std::string> ins[INSCOUNTER], int insNr)
{
	std::map<std::string, long long> M;
	int invokedTimes = 0;

	for (char i = 'a'; i <= 'h'; ++ i)
		M[std::to_string(i)] = 0;

	for (int i = 0; i >= 0 && i < insNr;) {
		if (ins[i][0] == "mul")
			invokedTimes ++;

		i += binaryOp(M, ins[i][0], ins[i][1], ins[i][2]);
	}

	return invokedTimes;
}

int solveTask2(std::vector<std::string> ins[INSCOUNTER], int insNr)
{
	int b = (std::stoi(ins[0][2]) * std::stoi(ins[4][2])) - std::stoi(ins[5][2]);
	int c = b - std::stoi(ins[7][2]);
	int g = 0;
	int h = 0;

	do {
		int f = 1;

		for (int d = std::stoi(ins[9][2]); d < b; ++ d)
			if (b % d == 0) {
				f = 0;
				break;
			}
	
		if (!f)
			++ h;

		g = b - c;
		b -= std::stoi(ins[30][2]);
	} while (g != 0);

	return h;
}


int main()
{
	std::ifstream fin("data.in");
	std::string line;
	std::vector<std::string> ins[INSCOUNTER];
	int insNr = 0;

	while (std::getline(fin, line))
		ins[insNr ++] = stringSplit(line);

	fin.close();

	std::cout << "The answer for part1 is: " << solveTask1(ins, insNr) << "\n";
	std::cout << "The answer for part2 is: " << solveTask2(ins, insNr) << "\n";
	
	return 0;
}