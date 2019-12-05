#include <iostream>
#include <fstream>

int run(std::string str, int iterations) {
	for (int i = 0; i < iterations; ++ i) {
		std::string tmp;

		for (size_t j = 0; j < str.size();) {
			size_t ind;

			for (ind = j; str[j] == str[ind]; ++ j);
		
			tmp += std::to_string(j - ind);
			tmp += str[ind];
		}

		str = tmp;
	}

	return str.size();
}

int main() {
	std::ifstream fin("data.in");
	std::string str;

	fin >> str;

	std::cout << "The answer for part1 is: " << run(str, 40) << "\n";
	std::cout << "The answer for part2 is: " << run(str, 50) << "\n";

	fin.close();

	return 0;
}