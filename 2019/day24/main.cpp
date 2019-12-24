#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <algorithm>

constexpr int PART2_STEPS = 200;
constexpr char BUG = '#';
constexpr char EMPTY = '.';
constexpr char LEVEL_TILE = '?';
constexpr int vert[] = {-1, 0, 1, 0};
constexpr int oriz[] = {0, 1, 0, -1};
constexpr int CENTER_X = 2;
constexpr int CENTER_Y = 2;

enum Direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT,
    DIR_MAX,
};

typedef std::vector<std::string> GRID;

GRID stepPart1(const GRID& grid) {
    GRID newGrid(grid);

    for (int i = 0; i < (int)grid.size(); ++ i) {
        for (int j = 0; j < (int)grid[i].size(); ++ j) {
            int adjacentBugs = 0;

            for (int k = 0; k < DIR_MAX; ++ k) {
                int newI = i + vert[k];
                int newJ = j + oriz[k];

                if (newI < 0 || newI >= (int)grid.size() ||
                    newJ < 0 || newJ >= (int)grid[i].size())
                {
                    continue;
                }

                adjacentBugs += (grid[newI][newJ] == BUG);
            }

            if (grid[i][j] == EMPTY && (adjacentBugs == 1 || adjacentBugs == 2)) {
                newGrid[i][j] = BUG;
            } else if (grid[i][j] == BUG && adjacentBugs == 1) {
                newGrid[i][j] = BUG;
            } else {
                newGrid[i][j] = EMPTY;
            }
        }
    }

    return newGrid;
}

int countBugsOnBorder(const GRID& grid, int dir) {
    int bugs = 0;

    switch (dir) {
    case UP:
        bugs = std::count_if(grid[0].begin(), grid[0].end(), [](char ch){ return ch == BUG; });
        break;
    case DOWN:
        bugs = std::count_if(grid.back().begin(), grid.back().end(), [](char ch){ return ch == BUG; });
        break;
    case RIGHT:
        bugs = std::count_if(grid.begin(), grid.end(), [](std::string str) { return str.back() == BUG; });
        break;
    case LEFT:
        bugs = std::count_if(grid.begin(), grid.end(), [](std::string str) { return str[0] == BUG; });
        break;
    default:
        std::cerr << "Invalid direction!\n";
        break;    
    }

    return bugs;
}

GRID stepPart2(const GRID& currLevel, const GRID& prevLevel, const GRID& nextLevel) {
    GRID newGrid(currLevel);

    for (int i = 0; i < (int)currLevel.size(); ++ i) {
        for (int j = 0; j < (int)currLevel[i].size(); ++ j) {
            int adjacentBugs = 0;

            for (int k = 0; k < DIR_MAX; ++ k) {
                int newI = i + vert[k];
                int newJ = j + oriz[k];

                if (newI < 0) {
                    adjacentBugs += (prevLevel[CENTER_Y - 1][CENTER_X] == BUG);
                } else if (newI >= (int)currLevel.size()) {
                    adjacentBugs += (prevLevel[CENTER_Y + 1][CENTER_X] == BUG);
                } else if (newJ < 0) {
                    adjacentBugs += (prevLevel[CENTER_Y][CENTER_X - 1] == BUG);
                } else if (newJ >= (int)currLevel[i].size()) {
                    adjacentBugs += (prevLevel[CENTER_Y][CENTER_X + 1] == BUG);
                } else if (currLevel[newI][newJ] == LEVEL_TILE) {
                    adjacentBugs += countBugsOnBorder(nextLevel, (k + 2) % DIR_MAX);
                } else {
                    adjacentBugs += (currLevel[newI][newJ] == BUG);
                }
            }

            if (currLevel[i][j] == EMPTY && (adjacentBugs == 1 || adjacentBugs == 2)) {
                newGrid[i][j] = BUG;
            } else if (currLevel[i][j] == BUG && adjacentBugs == 1) {
                newGrid[i][j] = BUG;
            } else if (currLevel[i][j] == LEVEL_TILE) {
                newGrid[i][j] = LEVEL_TILE;
            } else {
                newGrid[i][j] = EMPTY;
            }
        }
    }

    return newGrid;
}

int countBugs(const GRID& grid, std::function<int(int)> func) {
    int res = 0;

    for (int i = 0; i < (int)grid.size(); ++ i) {
        for (int j = 0; j < (int)grid[i].size(); ++ j) {
            res += ((grid[i][j] == BUG) ? func(i * grid.size() + j) : 0);
        }
    }

    return res;
}

int solvePart1(std::vector<std::string> grid) {
    std::set<GRID> states;

    while (states.find(grid) == states.end()) {
        states.insert(grid);
        grid = stepPart1(grid);
    }

    return countBugs(grid, [](int pos)->int { return 1 << pos;});
}

int solvePart2(std::vector<std::string> grid) {
    GRID emptyGrid(grid.size(), std::string(grid.back().size(), EMPTY));
    std::map<int, GRID> levels;
    int lower_bound = 0, upper_bound = 0;
    int res = 0;

    grid[CENTER_Y][CENTER_X] = emptyGrid[CENTER_Y][CENTER_X] = LEVEL_TILE;
    levels[0] = grid;

    for (int i = 0; i < PART2_STEPS; ++ i) {
        std::map<int, GRID> newLevels;

        lower_bound --;
        upper_bound ++;

        levels[lower_bound] = levels[upper_bound] = emptyGrid;

        newLevels[lower_bound] = stepPart2(emptyGrid, emptyGrid, levels[lower_bound + 1]);
        newLevels[upper_bound] = stepPart2(emptyGrid, levels[upper_bound - 1], emptyGrid);

        for (int i = lower_bound + 1; i < upper_bound; ++ i) {
            newLevels[i] = stepPart2(levels[i], levels[i - 1], levels[i + 1]);
        }

        levels = newLevels;
    }

    for (const auto& x : levels) {
        res += countBugs(x.second, [](int pos)->int { return 1; });
    }

    return res;
}

int main() {
    std::ifstream fin("data.in");
    GRID grid;
    std::string str;
    int sol1 = 0;

    while (std::getline(fin, str)) {
        if (str.empty()) {
            break;
        }

        grid.push_back(str);
    }

    std::cout << "The answer for part1 is: " << solvePart1(grid) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(grid) << "\n";

    fin.close();

    return 0;
}
