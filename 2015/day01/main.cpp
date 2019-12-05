#include <iostream>
#include <fstream>

int main()
{
    std::ifstream fin("data.in");
    std::string str;
    int floor = 0;
    int basementCount = -1;

    std::getline(fin, str);

    for (size_t i = 0; i < str.size(); ++ i) {
        if (str[i] == '(') {
            ++ floor;
        } else {
            -- floor;
        }

        if (basementCount == -1 && floor == -1) {
            basementCount = i + 1;         
        }
    }

    std::cout << "The answer for part1 is: " << floor << "\n";
    std::cout << "The answer for part2 is: " << basementCount << "\n";

    fin.close();

    return 0;
}
