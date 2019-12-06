#include <iostream>
#include <fstream>
#include <map>

static const std::map<std::string, int> escapes{
    {"\\\\", 1},
    {"\\\"", 1},
    {"\\x",  3},
};

int main()
{
    std::ifstream fin("data.in");
    std::string str;
    int sol1 = 0, sol2 = 0;

    while (std::getline(fin, str)) {
        sol1 += 2;
        sol2 += 2;

        for (const auto& ch : str) {
            sol2 += (ch == '\\' || ch == '"');
        }
    
        for (const auto& e : escapes) {
            size_t pos = 0;

            while ((pos = str.find(e.first, pos)) != std::string::npos) {
                sol1 += e.second;
                str.replace(pos, e.first.size(), e.first.size(), '_');
                pos = pos + e.first.size();
            }
        }
    }

    std::cout << "The answer for part1 is: " << sol1 << "\n";
    std::cout << "The answer for part2 is: " << sol2 << "\n";

    fin.close();

    return 0;
}
