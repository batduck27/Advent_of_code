#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>

#include <stdio.h>

#define KMOD        (int)(23)
#define LEFT_SHIFT  (int)(7)
#define RIGHT_SHIFT (int)(2)

void next(std::list<int>& L, std::list<int>::iterator& it) {
    ++ it;

    if (it == L.end()) {
        it = L.begin();
    }
}

void prev(std::list<int>& L, std::list<int>::iterator& it) {
    if (it == L.begin()) {
        it = L.end();
    }

    -- it;
}

void leftShift(std::list<int>& L, std::list<int>::iterator& it) {
    for (int i = 0; i < LEFT_SHIFT; ++ i) {
        prev(L, it);
    }
}

void rightShift(std::list<int>& L, std::list<int>::iterator& it) {
    for (int i = 0; i < RIGHT_SHIFT; ++ i) {
        next(L, it);
    }
}

long long solve(int players, int points) {
    std::list<int> game {0};
    std::list<int>::iterator currPos = game.begin();
    
    long long max = 0;
    std::vector<long long> V(players, 0);

    for (int i = 1, p = 0; i <= points; ++ i, p = (p + 1) % players) {
        if (i % KMOD == 0) {
            leftShift(game, currPos);

            max = std::max(max, (V[p] += i * 1LL + *currPos * 1LL));
            currPos = game.erase(currPos);
        } else {
            rightShift(game, currPos);

            currPos = game.insert(currPos, i);
        }

    }

    return max;
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    int players, points;

    std::getline(fin, str);
    sscanf(str.c_str(), "%d players; last marble is worth %d points", &players, &points);

    std::cout << "The answer for part1 is: " << solve(players, points) << "\n";
    std::cout << "The answer for part2 is: " << solve(players, points * 100) << "\n";

    fin.close();

    return 0;
}