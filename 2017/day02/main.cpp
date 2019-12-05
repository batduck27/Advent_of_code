#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

static const int INF = (1 << 30);
static const std::string numbers = "0123456789";

int getNumber(std::string str, size_t& ind)
{
    int nr = 0;

    for(; ind < str.size() && str[ind] >= '0' && str[ind] <= '9'; ++ ind) {
        nr = nr * 10 + str[ind] - '0';
    }

    return nr;
}

int main()
{
    std::string buffer;
    std::ifstream fin("data.in");
    int checksum1 = 0;
    int checksum2 = 0;

    while (getline(fin, buffer)) {
        int min = INF, max = 0;
        int evenlyDiv = 0;
        size_t ind = buffer.find_first_of(numbers);
        std::vector<int> V;

        while (ind != std::string::npos) {
            V.push_back(getNumber(buffer, ind));
            ind = buffer.find_first_of(numbers, ind + 1);
        }

        for (size_t i = 0; i < V.size(); ++ i) {
            min = std::min(min, V[i]);
            max = std::max(max, V[i]);

            for (size_t j = 0; j < V.size(); ++ j) {
                if (i != j && (V[j] % V[i] == 0)) {
                    evenlyDiv = V[j] / V[i];
                }
            }
        }

        checksum1 += (max - min);
        checksum2 += evenlyDiv;
    }

    std::cout << "The answer for part1 is: " << checksum1 << "\n";
    std::cout << "The answer for part2 is: " << checksum2 << "\n";

    fin.close();

    return 0;
}