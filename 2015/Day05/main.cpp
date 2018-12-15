#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

const std::string vowels = "aeiou";
const std::vector<std::string> pairs {"ab", "cd", "pq", "xy"};

int check1(const std::string& str) {
    int rule1 = 0, rule2 = 0, rule3 = 0;

    for (const auto& v : vowels) {
        rule1 += std::count(str.begin(), str.end(), v);
    }

    for (size_t i = 1; i < str.size(); ++ i) {
        if (str[i - 1] == str[i]) {
            rule2 ++;
        }
    }

    for (const auto& p : pairs) {
        if (str.find(p) != std::string::npos) {
            rule3 ++;
        }
    }

    return rule1 >= 3 && rule2 > 0 && rule3 == 0;
}

int check2(const std::string& str) {
    int rule1 = 0, rule2 = 0;

    for (size_t i = 1; i < str.size() - 1; ++ i) {
        if (str[i - 1] == str[i + 1]) {
            rule1 ++;
        }

        if (str.find(str.substr(i - 1, 2), i + 1) != std::string::npos) {
            rule2 ++;
        }
    }

    return rule1 > 0 && rule2 > 0;
}

int main()
{
    std::ifstream fin("data.in");
    std::string str;
    int res1 = 0, res2 = 0;

    while (std::getline(fin, str)) {
        res1 += check1(str);
        res2 += check2(str);
    }

    std::cout << "The answer for part1 is: " << res1 << "\n";
    std::cout << "The answer for part2 is: " << res2 << "\n";

    fin.close();

    return 0;
}
