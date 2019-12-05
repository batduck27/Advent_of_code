#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

static const int INF = (1 << 30);

typedef struct {
    int x, y;
    bool isInfinite = false;
    int currSize = 0;
} Point;

std::vector<Point> V;

int Manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int main() {
    std::ifstream fin("data.in");
    std::string str;

    int maxX = 0, maxY = 0;

    while (std::getline(fin, str)) {
        Point p;

        sscanf(str.c_str(), "%d, %d", &p.x, &p.y);

        V.push_back(p);
    
        maxX = std::max(p.x, maxX);
        maxY = std::max(p.y, maxY);
    }

    int res2 = 0;

    for (int i = -50; i <= maxY + 50; ++ i) {
        for (int j = -50; j <= maxX + 50; ++ j) {
            int minDist = INF;
            int point = 0;
            bool overlap = false;
            int distToAll = 0;

            // get the closest point
            for (size_t k = 0; k < V.size(); ++ k) {
                int dist = Manhattan(j, i, V[k].x, V[k].y);

                distToAll += dist;

                if (dist < minDist) {
                    minDist = dist;
                    point = k;
                    overlap = false;
                } else if (dist == minDist) {
                    overlap = true;
                }
            }

            if (distToAll < 10000) {
                res2 ++;
            }

            if (!overlap) {
                V[point].currSize ++;
            }

            if (i == 0 || j == 0 || i == maxY || j == maxX) {
                V[point].isInfinite = true;
            }
        } 
    }

    int max = 0;

    for (size_t i = 0; i < V.size(); ++ i) {
        if (!V[i].isInfinite) {
            max = std::max(max, V[i].currSize);
        }
    }

    std::cout << "The answer for part1 is: " << max << "\n";
    std::cout << "The answer for part2 is: " << res2 << "\n";
    
    fin.close();

    return 0;
}