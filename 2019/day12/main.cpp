#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <numeric>

#include <stdio.h>

static const int N_DIMENSION = 3;
static const int PART1_STEPS = 1000;

enum Indeces {
    X_COORD = 0,
    Y_COORD,
    Z_COORD,
    X_VELOC,
    Y_VELOC,
    Z_VELOC,
};

void simulateStep(std::vector<std::vector<int>> &moons) {
    std::vector<std::vector<int>> curr = moons;

    for (int i = 0; i < (int)moons.size(); ++ i) {
        for (int j = 0; j < (int)moons.size(); ++ j) {
            if (i == j) {
                continue;
            }

            for (int axis = 0; axis < N_DIMENSION; ++ axis) {
                if (curr[i][axis] < curr[j][axis]) {
                    moons[i][axis + N_DIMENSION] ++;
                } else if (curr[i][axis] > curr[j][axis]) {
                    moons[i][axis + N_DIMENSION] --;
                }
            }
        }

        for (int axis = 0; axis < N_DIMENSION; ++ axis) {
            moons[i][axis] += moons[i][axis + N_DIMENSION];
        }
    }
}

int getTotalEnergy(std::vector<std::vector<int>> moons) {
    int total = 0;

    for (const auto& moon : moons) {
        int pot = abs(moon[X_COORD]) + abs(moon[Y_COORD]) + abs(moon[Z_COORD]);
        int kin = abs(moon[X_VELOC]) + abs(moon[Y_VELOC]) + abs(moon[Z_VELOC]);

        total += pot * kin;
    }

    return total;
}

int solvePart1(std::vector<std::vector<int>> moons) {
    for (int i = 0; i < PART1_STEPS; ++ i) {
        simulateStep(moons);
    }

    return getTotalEnergy(moons);
}

std::vector<int> getAxisState(const std::vector<std::vector<int>>& moons, int axis) {
    std::vector<int> tmp;

    for (const auto& moon : moons) {
        tmp.push_back(moon[axis]);
        tmp.push_back(moon[axis + N_DIMENSION]);
    }

    return tmp;
}

long long solvePart2(std::vector<std::vector<int>> moons) {
    std::vector<std::set<std::vector<int>>> coordSets{N_DIMENSION};
    std::vector<long long> stepsNeeded(N_DIMENSION, 0);
    int coordsFound = 0;

    for (int axis = 0; axis < N_DIMENSION; ++ axis) {
        coordSets[axis].insert(getAxisState(moons, axis));
    }

    while (coordsFound < N_DIMENSION) {
        simulateStep(moons);

        for (int axis = 0; axis < N_DIMENSION; ++ axis) {
            std::vector<int> tmp;

            if (stepsNeeded[axis] != 0) {
                continue;
            }

            tmp = getAxisState(moons, axis);

            if (coordSets[axis].find(tmp) != coordSets[axis].end()) {
                stepsNeeded[axis] = (long long)coordSets[axis].size();
                coordsFound ++;
            } else {
                coordSets[axis].insert(tmp);
            }
        }
    }

    return std::lcm(std::lcm(stepsNeeded[X_COORD], stepsNeeded[Y_COORD]), stepsNeeded[Z_COORD]);
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    std::vector<std::vector<int>> moons;

    while (std::getline(fin, str)) {
        int x, y, z;

        sscanf(str.c_str(), "<x=%d, y=%d, z=%d>", &x, &y, &z);
        moons.push_back(std::vector<int>{x, y, z, 0, 0, 0});
    }

    std::cout << "The answer for part 1 is: " << solvePart1(moons) << "\n";
    std::cout << "The answer for part 2 is: " << solvePart2(moons) << "\n";

    return 0;
}
