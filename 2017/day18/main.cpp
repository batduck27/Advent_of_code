#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <queue>
#include <thread>

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
	if (op[0] >= 'a' && op[0] <= 'z') {
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

	if (ins == "add") {
		M[op1] += getValue(M, op2);
		return 1;
	}

	if (ins == "mul") {
		M[op1] *= getValue(M, op2);
		return 1;
	}

	if (ins == "mod") {
		M[op1] %= getValue(M, op2);
		return 1;
	}

	if (ins == "jgz") {
		if (getValue(M, op1) > 0)
			return getValue(M, op2);

		return 1;
	}

	return 1;
}

bool unaryOp(std::map<std::string, long long>& M, int& last, std::string ins, std::string op)
{
	if (ins == "snd") {
		last = getValue(M, op);
		return false;
	}

	if (ins == "rcv") {
		if (getValue(M, op))
			return true;

		return false;
	}
}

void duet(const std::vector<std::string> ins[INSCOUNTER],
		 std::map<std::string, long long>& M,
		 std::queue<int>& takeFrom, std::queue<int>& putIn,
		 int insNr, int& lastIns, int& sndCounter)
{
	for (int i = lastIns; i >= 0 && i < insNr; lastIns = i) {
		if (ins[i].size() == 2) {
			if (ins[i][0] == "snd") {
				putIn.push(getValue(M, ins[i][1]));
				++ sndCounter;
			}
			else {
				if (takeFrom.empty())
					return;

				M[ins[i][1]] = takeFrom.front();
				takeFrom.pop();
			}

			++ i;
		}
		else
			i += binaryOp(M, ins[i][0], ins[i][1], ins[i][2]);
	}
}

int solveTask1(std::vector<std::string> ins[INSCOUNTER], int insNr)
{
	std::map<std::string, long long> M;
	int last = -1;

	for (int i = 0; i >= 0 && i < insNr;) {
		if (ins[i].size() == 2) {
			if (unaryOp(M, last, ins[i][0], ins[i][1]))
				break;

			++ i;
		}
		else
			i += binaryOp(M, ins[i][0], ins[i][1], ins[i][2]);
	}

	return last;
}

int solveTask2(std::vector<std::string> ins[INSCOUNTER], int insNr)
{
	std::queue<int> Q0, Q1;
	std::map<std::string, long long> M0, M1;
	int sndCounter0 = 0, sndCounter1 = 0;
	int lastIns0 = 0, lastIns1 = 0;

	M0["p"] = 0;
	M1["p"] = 1;

	while (true) {
		duet(ins, std::ref(M0), std::ref(Q1), std::ref(Q0), insNr, std::ref(lastIns0), std::ref(sndCounter0));
		duet(ins, std::ref(M1), std::ref(Q0), std::ref(Q1), insNr, std::ref(lastIns1), std::ref(sndCounter1));
	
		if (Q0.empty() && Q1.empty())
			break;
	}

	return sndCounter1;
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