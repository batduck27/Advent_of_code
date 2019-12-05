#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

class State {
public:
	char name;

	int value[2];
	int step[2];
	char stateToContinue[2];
};

char startState;
int stepsToPass;
std::vector<State> S;

std::vector<State>::iterator getState(char state)
{
	for (std::vector<State>::iterator it = S.begin(); it != S.end(); ++ it)
		if ((*it).name == state)
			return it;
}

void readInput()
{
	std::ifstream fin("data.in");
	std::string line;

	std::getline(fin, line);
	startState = line[line.size() - 2];

	std::getline(fin, line);
	{
		size_t a = line.find_first_of("0987654321");
		size_t b = line.find_first_of(" ", a + 1);

		stepsToPass = std::stoi(line.substr(a, b - a));
	}

	State tmp;

	while (std::getline(fin, line))
		if (line.size() > 1) {
			tmp.name = line[line.size() - 2];

			for (int k = 2; k; -- k) {
				int i;

				std::getline(fin, line);
				i = line[line.size() - 2] - '0';
			
				std::getline(fin, line);
				tmp.value[i] = line[line.size() - 2] - '0';

				std::getline(fin, line);

				if (line[line.size() - 3] == 'h')
					tmp.step[i] = 1;
				else
					tmp.step[i] = -1;


				std::getline(fin, line);
				tmp.stateToContinue[i] = line[line.size() - 2];	

			}

			S.push_back(tmp);
		}

	fin.close();
}

int main()
{
	readInput();

	std::map<int, int> M;
	int pos = 0, steps = 0;
	int currState = startState;	

	while (steps < stepsToPass) {
		auto it = getState(currState);
		int i = M[pos];

		M[pos] = (*it).value[i];
		pos += (*it).step[i];
		currState = (*it).stateToContinue[i];

		++ steps;
	}

	int cnt = 0;

	for (const auto& x : M)
		if (x.second == 1)
			++ cnt;

	std::cout << "The answer is: " << cnt << "\n";

	return 0;
}