#include <fstream>
#include <iostream>
#include <unordered_map>

static const size_t PART1_WINDOW_SIZE = 4;
static const size_t PART2_WINDOW_SIZE = 14;

int findStartMarker(const std::string &datastream, size_t windowSize) {
    std::unordered_map<char, int> rollingWindow;
    size_t i;

    for (i = 0; i < windowSize - 1; ++ i) {
        rollingWindow[datastream[i]] ++;
    }

    for (; i < datastream.size(); ++ i) {
        char windowStartChar = datastream[i - windowSize + 1];

        rollingWindow[datastream[i]] ++;

        if (rollingWindow.size() >= windowSize) {
            return i + 1;
        }

        rollingWindow[windowStartChar] --;
        if (rollingWindow[windowStartChar] <= 0) {
            rollingWindow.erase(windowStartChar);
        }
    }

    return -1;
}

int solvePart1(const std::string &datastream) {
    return findStartMarker(datastream, PART1_WINDOW_SIZE);
}

int solvePart2(const std::string &datastream) {
    return findStartMarker(datastream, PART2_WINDOW_SIZE);
}

int main() {
    std::ifstream fin("data.in");
    std::string datastream;

    std::getline(fin, datastream, '\n');

    std::cout << "The answer for part1 is: " << solvePart1(datastream) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(datastream) << "\n";

    fin.close();

    return 0;
}