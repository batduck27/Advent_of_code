#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const int NMAX = 1000005;

int part1(const int& presentsNumber) {
	std::vector<int> v(NMAX, 10);
	
	for (int i = 2; i < NMAX; ++ i) {
		for (int j = i; j < NMAX; j += i) {
			v[j] += i * 10;
		}
	}

	auto it = std::find_if(v.begin(),
	                       v.end(),
	                       [presentsNumber](int n)->bool {
	                           return n >= presentsNumber;
	                       }); 

	return it - v.begin();
}

int part2(const int& presentsNumber) {
	std::vector<int> v(NMAX, 10);
	
	for (int i = 2; i < NMAX; ++ i) {
		for (int j = i; j < std::min(NMAX, i * 50); j += i) {
			v[j] += i * 11;
		}
	}

	auto it = std::find_if(v.begin(),
	                       v.end(),
	                       [presentsNumber](int n)->bool {
	                           return n >= presentsNumber;
	                       }); 

	return it - v.begin();
}

int main() {
	std::ifstream fin("data.in");
	int presentsNumber;

	fin >> presentsNumber;


	std::cout << "The answer for part1 is: " << part1(presentsNumber) << "\n";
	std::cout << "The answer for part2 is: " << part2(presentsNumber) << "\n";

	fin.close();

	return 0;
}