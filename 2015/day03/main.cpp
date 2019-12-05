#include <iostream>
#include <fstream>
#include <set>
#include <utility>

int solve(const std::string& str, bool part2 = false) {
    std::set<std::pair<int, int>> vis;
    int hx = 0, hy = 0;
    int rx = 0, ry = 0;

    vis.insert(std::make_pair(hx, hy));

    for (size_t i = 0; i < str.size(); ++ i) {
        int *x, *y;

        if (part2 && i % 2 == 1) {
            x = &rx;
            y = &ry;
        } else {
            x = &hx;
            y = &hy;
        }

        switch (str[i]) {
            case '^':
                ++ (*y);
                break;
            case '>':
                ++ (*x);
                break;
            case 'v':
                -- (*y);
                break;
            case '<':
                -- (*x);
                break;
            default:
                return -1;
        }

        vis.insert(std::make_pair(*x, *y));
    }

    return vis.size();
}

int main()
{
    freopen("data.in", "r", stdin);
    std::ifstream fin("data.in");
    std::string str;

    std::getline(fin, str);

    std::cout << "The answer for part1 is: " << solve(str) << "\n";
    std::cout << "The answer for part2 is: " << solve(str, true) << "\n";

    fin.close();

    return 0;
}