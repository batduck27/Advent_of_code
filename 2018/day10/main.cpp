#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>

#include <stdio.h>
#include <stdlib.h>

#define INF        (int)(1 << 29)
#define LETTERSIZE (int)(10)

typedef struct {  
    int x, y, vx, vy;
} Point;

std::vector<Point> V;

void tick(int& minY, int& minX, int& maxY, int& maxX, int factor = 1) {
    minX = minY = INF;
    maxX = maxY = -INF;
    
    for (auto& p : V) {
        p.x += factor * p.vx;
        p.y += factor * p.vy;

        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
    }
}

int main() {
    std::ifstream fin("data.in");
    std::string str;

    while (std::getline(fin, str)) {
        Point p;
        sscanf(str.c_str(), "position=< %d,  %d> velocity=<%d, %d>", &p.x, &p.y, &p.vx, &p.vy);
        V.push_back(p);
    }

    int steps = abs(V[0].y/ V[0].vy);
    int minX, minY, maxX, maxY;

    tick(minY, minX, maxY, maxX, steps);

    for (; abs(maxY - minY) > LETTERSIZE; ++ steps) {
        tick(minY, minX, maxY, maxX);
    }   

    std::set<std::pair<int, int>> S;

    for (const auto& p : V) {
        S.insert(std::make_pair(p.x, p.y));
    }

    std::cout << "The answer for part 1 is: " << "\n\n";

    for (int i = minY; i <= maxY; ++ i) {
        for (int j = minX; j <= maxX; ++ j) {
            if (S.find(std::make_pair(j, i)) != S.end()) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }

        std::cout << "\n";
    }

    std::cout << "\nThe answer for part 2 is: " << steps << "\n";

    fin.close();

    return 0;
}