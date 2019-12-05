#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <vector>

static const std::string alphabet = "qwertyuiopasdfghjklzxcvbnm";

std::string getWord(std::string str, size_t& ind)
{
    size_t first = ind;

    while (ind < str.size() && str[ind] >= 'a' && str[ind] <= 'z') {
        ++ ind;
    }

    return str.substr(first, ind - first);
}

void wordSplit(std::string str, std::vector<std::string>& W)
{
    size_t ind = str.find_first_of(alphabet);

    while (ind != std::string::npos) {
        W.push_back(getWord(str, ind));
        ind = str.find_first_of(alphabet, ind + 1);
    }
}

int getValidity(std::string str, std::vector<std::string> W, bool isTask2)
{
    std::unordered_map<std::string, int> H;

    for (size_t i = 0; i < W.size(); ++ i) {
        if (isTask2 == true) {
            sort(W[i].begin(), W[i].end());
        }

        std::unordered_map<std::string, int>::const_iterator IT = H.find(W[i]);

        if (IT != H.end()) {
            return 0;
        }

        H.insert({W[i], 1});
    }

    return 1;
}

int main()
{
    std::string buffer;
    std::ifstream fin("data.in");
    int validCounter1 = 0;
    int validCounter2 = 0;

    while (getline(fin, buffer)) {
        std::vector<std::string> W;
        
        wordSplit(buffer, W);

        validCounter1 += getValidity(buffer, W, false);
        validCounter2 += getValidity(buffer, W, true);
    }

    std::cout << "The answer for part1 is: " << validCounter1 << "\n";
    std::cout << "The answer for part2 is: " << validCounter2 << "\n";

    fin.close();

    return 0;
}