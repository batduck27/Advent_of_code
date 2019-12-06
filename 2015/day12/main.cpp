#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <stack>

std::vector<int> extractNumbers(const std::string& str) {
    static const std::string numbers = "-0123456789";
    std::vector<int> v;
    size_t start = str.find_first_of(numbers), end = start;

    while (start != std::string::npos) {
        end = str.find_first_not_of(numbers, start);
        v.push_back(std::stoi(str.substr(start, end - start)));
        start = str.find_first_of(numbers, end);
    }

    return v;
}

int getSum(std::string json, bool isPart2 = false) {
    std::vector<int> numbers;

    if (isPart2) {
        std::stack<size_t> S;

        for (size_t i = 0; i < json.size(); ++ i) {
            if (json[i] == '{') {
                S.push(i);
            } else if (json[i] == '}') {
                if (json.substr(S.top(), i - S.top()).find(":\"red\"") != std::string::npos) {
                    json.replace(S.top(), i - S.top() + 1, i - S.top() + 1, '_');
                }

                S.pop();
            } else {
                // do nothing
            }
        }
    }

    numbers = extractNumbers(json);

    return std::accumulate(numbers.begin(), numbers.end(), 0);
}

int main() {
    std::ifstream fin("data.in");
    std::string json;

    fin >> json;
    fin.close();

    std::cout << "The answer for part1 is: " << getSum(json) << "\n";
    std::cout << "The answer for part2 is: " << getSum(json, true) << "\n";

    return 0;
}
