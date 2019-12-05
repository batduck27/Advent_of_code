#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

static const int vert[] = {-1, 0, 1, 0};
static const int oriz[] = {0, 1, 0, -1};

void changeDirection(const std::vector<std::string>& maze, int i, int j, int& dir)
{
    for (int k = 0; k < 4; ++ k) {
        // ignore the opposite direction
        if (abs(k - dir) == 2) {
            continue;
        }

        int newI = i + vert[k];
        int newJ = j + oriz[k];

        if (maze[newI][newJ] != ' ') {
            dir = k;
            return;
        }
    }
}

int main()
{
    std::ifstream fin("data.in");
    std::vector<std::string> maze;
    std::string line;
    std::string order;
    int dir = DOWN;
    int steps = 0;
    size_t i = 0, j = 0;

    while (std::getline(fin, line)) {
        maze.push_back(line);
    }

    fin.close();

    j = maze[0].find_first_of("|");

    while (i >= 0 && i < maze.size() && j >= 0 && j < maze[i].size()) {
        if (maze[i][j] >= 'A' && maze[i][j] <= 'Z') {
            order.push_back(maze[i][j]);
        } else if (maze[i][j] == '+') {
            changeDirection(maze, i, j, dir);
        } else if (maze[i][j] == ' ') {
            break;
        }

        i += vert[dir];
        j += oriz[dir];
        ++ steps;
    }

    std::cout << "The answer for part1 is: " << order << "\n";
    std::cout << "The answer for part2 is: " << steps << "\n";
    
    return 0;
}