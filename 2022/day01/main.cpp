#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

static const size_t PART2_ELF_CNT = 3;

int main() {
    std::ifstream fin("data.in");
    std::vector<int> elfsCalories;
    std::string line;
    int currCaloriesSum = 0;

    while (std::getline(fin, line, '\n')) {
        if (line.empty()) {
            elfsCalories.push_back(currCaloriesSum);
            currCaloriesSum = 0;
            continue;
        }

        currCaloriesSum += std::stoi(line);
    }

    std::sort(elfsCalories.begin(), elfsCalories.end(), std::greater<int>());

    std::cout << "The answer for part1 is: " << elfsCalories.at(0) << "\n";
    std::cout << "The answer for part2 is: " <<
        std::accumulate(elfsCalories.begin(), elfsCalories.begin() + PART2_ELF_CNT, 0) << "\n";

    fin.close();

    return 0;
}