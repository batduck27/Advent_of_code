#include <iostream>
#include <algorithm>
#include <set>
#include <tuple>

#include <stdio.h>

typedef std::tuple<int, int, int, int> Pos;

enum RegionType {ROCKY, WET, NARROW};
enum Gears {NEITHER, TORCH, CLIMBING, GEAR_LAST};

const int NMAX = 1000;
const int INF = (1 << 30);
const int vert[] = {-1, 0, 1, 0};
const int oriz[] = {0, 1, 0, -1};
const long long XFACTOR = 16807;
const long long YFACTOR = 48271;
const long long MOD = 20183;

long long grid[NMAX][NMAX];
int type[NMAX][NMAX], best[NMAX][NMAX][3];

int solvePart2(int tX, int tY) { 
    static const int OUT = 100;
    std::set<Pos> S;
    S.insert(Pos(0, TORCH, 0, 0));

    best[0][0][TORCH] = -1;

    while (!S.empty()) {
        auto [currTime, currGear, currX, currY] = *(S.begin());
        S.erase(S.begin());

        for (int k = 0; k < 4; ++ k) {
            int newY = currY + vert[k];
            int newX = currX + oriz[k];

            if (newX < 0 || newX > tX + OUT || newY < 0 || newY > tY + OUT) {
                continue;
            }

            if (((type[newY][newX] + 1) % GEAR_LAST == currGear ||
                 (type[newY][newX] + 2) % GEAR_LAST == currGear) &&
                (best[newY][newX][currGear] == 0 || 
                 best[newY][newX][currGear] > currTime + 1)) {

                best[newY][newX][currGear] = currTime + 1;
                S.insert(Pos(currTime + 1, currGear, newX, newY));
            }
        }

        // switch to the other possible gear
        int tmp = (type[currY][currX] + 1) % GEAR_LAST;
        int newGear = (tmp == currGear) ? (tmp + 1) % GEAR_LAST : tmp;

        if (best[currY][currX][newGear] == 0 || best[currY][currX][newGear] > currTime + 7) {
            best[currY][currX][newGear] = currTime + 7;
            S.insert(Pos(currTime + 7, newGear, currX, currY));
        }       
    }

    return best[tY][tX][TORCH];
}

int main() {
    FILE* fin = fopen("data.in", "r");
    int depth, tY, tX;

    fscanf(fin, "depth: %d\ntarget: %d,%d", &depth, &tX, &tY);

    int res = 0;

    for (int y = 0; y < NMAX; ++ y) {
        for (int x = 0; x < NMAX; ++ x) {
            if ((x == 0 && y == 0) || (x == tX && y == tY)) {
                grid[y][x] = depth % MOD;
            } else if (x == 0) {
                grid[y][x] = ((y * YFACTOR) + depth) % MOD;
            } else if (y == 0) {
                grid[y][x] = ((x * XFACTOR) + depth) % MOD;
            } else {
                grid[y][x] = (grid[y - 1][x] * grid[y][x - 1] + depth) % MOD;
            }

            type[y][x] = grid[y][x] % 3;
            
            if (x <= tX && y <= tY) {
                res += type[y][x];
            }
        }
    }

    std::cout << "The answer for part1 is: " << res << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(tX, tY) << "\n";

    fclose(fin);

    return 0;
}