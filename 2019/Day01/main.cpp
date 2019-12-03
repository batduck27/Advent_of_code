#include <iostream>
#include <fstream>

int main() {
    std::ifstream fin("data.in");
    long long sol1 = 0, sol2 = 0, x;

    while (fin >> x) {
        x = x / 3 - 2;
        sol1 += x;

        while (x > 0) {
            sol2 += x;
            x = x / 3 - 2;
        }
    }

    std::cout << "The answer for part1 is: " << sol1 << "\n";
    std::cout << "The answer for part2 is: " << sol2 << "\n";

    fin.close();

    return 0;
}