#include <iostream>
#include <fstream>
#include <set>
#include <map>

int main() {
    std::ifstream fin("../input/day06.in");
    long long sol1 = 0, sol2 = 0, x;
    std::string str;

    std::set<char> S;
    std::map<char, int> M;
    int lines = 0;


    while (std::getline(fin, str)) {
        lines ++;

        if (str.empty()) {
            sol1 += S.size();
            S.clear();

            for (const auto &[ch, r] : M) {
                if (r == lines - 1) {
                    sol2++;
                }
            }

            M.clear();
            lines = 0;
            continue;
        }

        for (const auto & ch : str) {
            S.insert(ch);
            M[ch] ++;
        }
    }

    sol1 += S.size();
    S.clear();
    for (const auto &[ch, r] : M) {
        if (r == lines) {
            sol2++;
        }
    }

    M.clear();

    std::cout << "The answer for part1 is: " << sol1 << "\n";
    std::cout << "The answer for part2 is: " << sol2 << "\n";

    fin.close();

    return 0;
}
