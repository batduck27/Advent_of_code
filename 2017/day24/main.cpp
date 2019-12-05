#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

class Component {
public:
    int x, y;
    bool used = false;

    Component() 
    {
        this->x = 0;
        this->y = 0;
    }

    Component(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

int bestStrength, bestLength, bestStrByLgt;

void back(std::vector<Component>& V, int lastPort, int currStrength, int currLength)
{
    bestStrength = std::max(bestStrength, currStrength);

    if (currLength > bestLength) {
        bestStrByLgt = currStrength;
        bestLength = currLength;
    }

    if (currLength == bestLength) {
        bestStrByLgt = std::max(currStrength, bestStrByLgt);
    }

    for (auto& c : V) {
        if (!c.used && (c.x == lastPort || c.y == lastPort)) {
            c.used = true;
            back(V, (c.x == lastPort) ? c.y : c.x, currStrength + c.x + c.y, currLength + 1);
            c.used = false;
        }
    }
}

int main()
{
    std::ifstream fin("data.in");
    std::string line;
    std::vector<Component> V;

    while (std::getline(fin, line)) {
        size_t ind = line.find_first_of("/"); 
        V.push_back(Component(std::stoi(line.substr(0, ind)), std::stoi(line.substr(ind + 1))));
    }

    fin.close();

    back(V, 0, 0, 0);
    std::cout << "The answer for part1 is: " << bestStrength << "\n";
    std::cout << "The answer for part2 is: " << bestStrByLgt << "\n";
    
    return 0;
}