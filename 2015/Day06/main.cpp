#include <iostream>
#include <fstream>

#include <stdio.h>

const int N = 1000;

int gridI[N][N];
bool gridB[N][N];

void toogle(int x1, int y1, int x2, int y2)
{
    for (int i = x1; i <= x2; ++ i) {
        for (int j = y1; j <= y2; ++ j) {
            gridB[i][j] ^= 1;
            gridI[i][j] += 2;
        }
    }
}

void turn(int x1, int y1, int x2, int y2, bool state)
{
    int iState = (state == true) ? 1 : -1;

    for (int i = x1 ; i <= x2; ++ i) {
        for (int j = y1; j <= y2; ++ j) {
            gridB[i][j] = state;
            gridI[i][j] = std::max(gridI[i][j] + iState, 0);
        }
    }
}

int main()
{
    std::ifstream fin("data.in");
    std::string str;

    while (std::getline(fin, str)) {
        int x1, y1, x2, y2;

        if (sscanf(str.c_str(), "turn off %d,%d through %d,%d", &x1, &y1, &x2, &y2)) {
            turn(x1, y1, x2, y2, false);
        } else if (sscanf(str.c_str(), "turn on %d,%d through %d,%d", &x1, &y1, &x2, &y2)) {
            turn(x1, y1, x2, y2, true);
        } else if (sscanf(str.c_str(), "toggle %d,%d through %d,%d", &x1, &y1, &x2, &y2)) {
            toogle(x1, y1, x2, y2);
        } else {
            return -1;
        }
    }

    int res1 = 0, res2 = 0;

    for (int i = 0; i < N; ++ i) {
        for (int j = 0; j < N; ++ j) {
            res1 += (int)gridB[i][j];
            res2 += gridI[i][j];
        }
    }

    std::cout << "The answer for part1 is: " << res1 << "\n";
    std::cout << "The answer for part2 is: " << res2 << "\n";

    fin.close();

    return 0;
}

