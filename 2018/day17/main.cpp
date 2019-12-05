#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

#include <stdio.h>

#define NMAX    (int)(2000)
#define CLAY    (char)('#')
#define SAND    (char)('.')
#define S_WATER (char)('~')
#define F_WATER (char)('|')
#define STREAM  (char)('+')
#define MySet   std::set<std::pair<int, int>, std::greater<std::pair<int, int>>>

enum Direction {RIGHT, DOWN, LEFT};

const int INF = (1 << 30);
const int vert[] = {0, 1, 0};
const int oriz[] = {1, 0, -1};

std::vector<std::string> grid(NMAX, std::string(NMAX, '.'));

void fillWithClay(int x1, int y1, int x2, int y2) {
    for (int i = y1; i <= y2; ++ i) {
        for (int j = x1; j <= x2; ++ j) {
            grid[i][j] = CLAY;
        }
    }
}

void moveOnRow(int y, int& x, int step) {
    while ((grid[y][x] == SAND || grid[y][x] == F_WATER) &&
           (grid[y + 1][x] == CLAY || grid[y + 1][x] == S_WATER)) {
        x += step;
    }
}

void moveOnColumn(int& y, int x) {
    while (grid[y][x] == SAND) {
        ++ y;
    }
}

void fillRow(int y, int x, int& minX, int& maxX, MySet& fVert, MySet& tmp) {
    if (grid[y][x] != F_WATER) {
        // ignore if the cell was already filled
        return;
    }

    if (grid[y + 1][x] == F_WATER) {
        // the cell could become S_WATER
        /* v   <- the water wouldn't flow anymore if tmp wasn't used 
        ...||||||.
        .#~~#~~#|.
        .#~~~~~#|.
        .#######|.
        */
        tmp.insert(std::make_pair(y, x));
        return;
    }

    int leftX = x - 1, rightX = x + 1;

    moveOnRow(y, leftX, -1);
    moveOnRow(y, rightX, 1);

    char fillType = F_WATER;

    if (grid[y][leftX] == CLAY && grid[y][rightX] == CLAY) {
        fillType = S_WATER;
    }

    for (int i = leftX + 1; i < rightX; ++ i) {
        grid[y][i] = fillType;
    }

    minX = std::min(minX, leftX);
    maxX = std::max(maxX, rightX);

    if (grid[y][leftX] == SAND && grid[y + 1][leftX] == SAND) {
        fVert.insert(std::make_pair(y, leftX));
        grid[y][leftX] = F_WATER;
    }

    if (grid[y][rightX] == SAND && grid[y + 1][rightX] == SAND) {
        fVert.insert(std::make_pair(y, rightX));
        grid[y][rightX] = F_WATER;
    }
}

void fillColumn(int y, int x, int maxY, MySet& fOriz) {
    if (grid[y][x] == F_WATER) {
        fOriz.insert(std::make_pair(y, x));
    }

    ++ y;

    while (y <= maxY && grid[y][x] == SAND) {
        if (y < maxY) {
            fOriz.insert(std::make_pair(y, x));
        }

        grid[y][x] = F_WATER;
        ++ y;
    }
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    int maxX = -INF, minX = INF, minY = INF, maxY = -INF;

    while (std::getline(fin, str)) {
        char coord1, coord2;
        int nr1, nr2, nr3;

        sscanf(str.c_str(), "%c=%d, %c=%d..%d", &coord1, &nr1, &coord2, &nr2, &nr3);

        if (coord1 == 'x') {
            fillWithClay(nr1, nr2, nr1, nr3);
            minX = std::min(minX, nr1);
            maxX = std::max(maxX, nr1);
            minY = std::min(minY, nr2);
            maxY = std::max(maxY, nr3);
        } else {
            fillWithClay(nr2, nr1, nr3, nr1);
            minX = std::min(minX, nr2);
            maxX = std::max(maxX, nr3);
            minY = std::min(minY, nr1);
            maxY = std::max(maxY, nr1);
        }
    }

    MySet fVert, fOriz, tmp;

    grid[0][500] = STREAM;
    fVert.insert(std::make_pair(0, 500));

    while (!fVert.empty()) {
        // water goes down...
        for (const auto& p : fVert) {
            fillColumn(p.first, p.second, maxY, fOriz);
        }
        
        fVert.clear();

        // and fill horizontally
        for (const auto& p : fOriz) {
            fillRow(p.first, p.second, minX, maxX, fVert, tmp);
        }

        // tmp stores the cells with F_WATER which could become S_WATER
        fOriz = tmp;
        tmp.clear();
    }


    int res1 = 0, res2 = 0;

    for (int i = minY; i <= maxY; ++ i) {
        for (int j = minX; j <= maxX; ++ j) {
            res1 += (grid[i][j] == S_WATER || grid[i][j] == F_WATER);
            res2 += (grid[i][j] == S_WATER);
        }
    }

    std::cout << "The answer for part1 is: " << res1 << "\n";
    std::cout << "The answer for part2 is: " << res2 << "\n";

    fin.close();

    return 0;
}