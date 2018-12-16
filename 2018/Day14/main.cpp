#include <iostream>
#include <fstream>

void tick(std::string& v, int& elf1, int& elf2) {
	v += std::to_string(v[elf1] - '0' + v[elf2] - '0');

	elf1 = (elf1 + 1 + v[elf1] - '0') % v.size();
	elf2 = (elf2 + 1 + v[elf2] - '0') % v.size();
}

std::string part1(std::string& v, int& elf1, int& elf2, int input) {
	while (v.size() < (size_t)input + 10) {
		tick(v, elf1, elf2);
	}

	return v.substr(input, 10);
}

int part2(std::string& v, int& elf1, int& elf2, const std::string& input) {
	if (v.find(input) != std::string::npos) {
		return v.find(input);
	}

	while (v.substr(v.size() - input.size() - 1, input.size()) != input) {
		tick(v, elf1, elf2);
	}

	return v.size() - input.size() - 1;
}

int main() {
	std::ifstream fin("data.in");
	std::string v = "37";
	std::string input;
	int elf1 = 0, elf2 = 1;

	fin >> input;

	std::cout << "The answer for part1 is: " << part1(v, elf1, elf2, std::stoi(input)) << "\n";
	std::cout << "The answer for part2 is: " << part2(v, elf1, elf2, input) << "\n"; 

	fin.close();

	return 0;
}