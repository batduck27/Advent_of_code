#include <iostream>
#include <fstream>

#include "md5.h"

const std::string part1(5, '0');
const std::string part2(6, '0');

int main()
{
    std::ifstream fin("data.in");
    std::string str;
    int res1 = -1, res2 = -1;

    std::getline(fin, str);

    for (int i = 1; res1 == -1 || res2 == -1; ++ i) {
        std::string tmp = md5(str + std::to_string(i));

        if (res1 == -1 && part1 == tmp.substr(0, 5)) {
            res1 = i;
        }

        if (res2 == -1 && part2 == tmp.substr(0, 6)) {
            res2 = i;
        }
    }

    std::cout << "The answer for part1 is: " << res1 << "\n";
    std::cout << "The answer for part2 is: " << res2 << "\n";

    fin.close();

    return 0;
}
