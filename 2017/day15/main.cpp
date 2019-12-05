#include <iostream>
#include <fstream>

static const int COUNTER1 = 40000000;
static const int COUNTER2 = 5000000;
static const int lastBits = (1 << 16);
static const long long factorA = 16807;
static const long long factorB = 48271;
static const long long MOD = 2147483647;
static const std::string numbers = "0123456789";

int solveTask1(long long A, long long B)
{
	int judge = 0;

	for (int i = 0; i < COUNTER1; ++ i) {
		A = (A * factorA) % MOD;
		B = (B * factorB) % MOD;

		if (A % lastBits == B % lastBits)
			++ judge;
	}

	return judge;
}

int solveTask2(long long A, long long B)
{
	int judge = 0;

	for (int i = 0; i < COUNTER2; ++ i) {
		A = (A * factorA) % MOD;
		B = (B * factorB) % MOD;

		while (A % 4)
			A = (A * factorA) % MOD;

		while (B % 8)
			B = (B * factorB) % MOD;

		if (A % lastBits == B % lastBits)
			++ judge;
	}

	return judge;
}

int main()
{
	std::ifstream fin("data.in");
	std::string line;
	long long A, B;

	std::getline(fin, line);
	A = (long long)(std::stoi(line.substr(line.find_first_of(numbers))));
	std::getline(fin, line);
	B = (long long)(std::stoi(line.substr(line.find_first_of(numbers))));

	std::cout << "The answer for part1 is: " << solveTask1(A, B) << "\n";
	std::cout << "The answer for part2 is: " << solveTask2(A, B) << "\n";

	return 0;
}