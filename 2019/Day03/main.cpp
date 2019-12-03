#include <iostream>
#include <fstream>
#include <vector>
#include <map>

typedef std::pair<int, int> II;

static const int INF = (1 << 30);
static const int WIRE_NO = 2;
static const II ORIGIN = II(0, 0);

static std::vector<std::string> tokenize(const std::string str) {
    static const std::string delim = " ,";
    std::vector<std::string> words;
    size_t start = 0, end = 0;

    while (start != std::string::npos) {
        end = str.find_first_of(delim, start);
        words.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delim, end);
    }

    return words;
}

static int Manhattan(II coord1, II coord2) {
    auto [y1, x1] = coord1;
    auto [y2, x2] = coord2;

    return abs(x1 - x2) + abs(y1 - y2);
}

std::map<II, int> usedCoords;
std::map<II, int> coordSteps;

void getWire(std::istream &fin, int wireNo) {
    std::string str;
    std::vector<std::string> words;
    int totalSteps = 0;
    int y = ORIGIN.first, x = ORIGIN.second;

    std::getline(fin, str);
    words = tokenize(str);

    for (const auto& w : words) {
        int oriz = 0, vert = 0;
        int steps = std::stoi(w.substr(1));

        if (w[0] == 'R') {
            oriz = 1;
        } else if (w[0] == 'L') {
            oriz = -1;
        } else if (w[0] == 'U') {
            vert = -1;
        } else if (w[0] == 'D') {
            vert = 1;
        } 

        while (steps) {
            y += vert;
            x += oriz;

            ++ totalSteps;
            -- steps;

            usedCoords[II(y, x)] |= wireNo;
            coordSteps[II(y, x)] += totalSteps;
        }
    }
}

int main() {
    int minDist = INF, minSteps = INF;
    std::ifstream fin("data.in");
    
    for (int i = 1; i <= WIRE_NO; ++ i) {
        getWire(fin, i);
    }

    for (const auto& entry : usedCoords) {
        II currCoord = entry.first;
        int overlap = entry.second;

        if (overlap > (1 << (WIRE_NO - 1))) {
            minDist  = std::min(minDist, Manhattan(ORIGIN, currCoord));
            minSteps = std::min(minSteps, coordSteps[currCoord]);
        }
    }

    std::cout << "The answer for part1 is: " << minDist<< "\n";
    std::cout << "The answer for part2 is: " << minSteps << "\n";

    fin.close();

    return 0;
}