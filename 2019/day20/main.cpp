#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <algorithm>
#include <cctype>

#define WALL '#'
#define PASSAGE '.'

typedef std::pair<int, int> II;
typedef std::tuple<int, int, int> III;

static II dirs[] = {II(-1, 0), II(0, 1), II(1, 0), II(0, -1)};
static const int INF = (1 << 30);

std::vector<std::string> maze;
std::map<std::string, std::vector<II>> portals;
std::map<II, std::string> coordsToPortals;

II operator+(const II& l, const II&r) {
    return II(l.first + r.first, l.second + r.second);
}

static bool isOuterLayer(const II& coords) {
    return (coords.first == 2 || coords.first == (int)maze.size() - 3 ||
            coords.second == 2 || coords.second == (int)maze.back().size() - 3);
}

static void getPortals() {
    for (int i = 1; i < (int)maze.size() - 1; ++ i) {
        for (int j = 1; j < (int)maze[i].size() - 1; ++ j) {
            if (isupper(maze[i][j])) {
                std::string portalName = "";
                II coords;

                if (maze[i - 1][j] == PASSAGE) {
                    // UP
                    portalName = std::string{maze[i][j], maze[i + 1][j]};
                    coords = II(i - 1, j);
                } else if (maze[i][j + 1] == PASSAGE) {
                    // RIGHT
                    portalName = std::string{maze[i][j - 1], maze[i][j]};
                    coords = II(i, j + 1);
                } else if (maze[i + 1][j] == PASSAGE) {
                    // DOWN
                    portalName = std::string{maze[i - 1][j], maze[i][j]};
                    coords = II(i + 1, j);
                } else if (maze[i][j - 1] == PASSAGE) {
                    // LEFT
                    portalName = std::string{maze[i][j], maze[i][j + 1]};
                    coords = II(i, j - 1);
                }

                if (!portalName.empty()) {
                    portals[portalName].push_back(coords);
                    coordsToPortals[coords] = portalName;
                }
            }
        }
    }
}

int BFS(II start, II target, bool part2 = false) {
    std::queue<III> Q;
    int steps[portals.size()][maze.size()][maze.back().size()];

    for (int i = 0; i < (int)portals.size(); ++ i) {
        for (int j = 0; j < (int)maze.size(); ++ j) {
            for (int k = 0; k < (int)maze.back().size(); ++ k) {
                steps[i][j][k] = INF;
            }
        }
    }

    Q.push(III(start.first, start.second, 0));
    steps[0][start.first][start.second] = 0;

    while (!Q.empty()) {
        auto [y, x, recursionLevel] = Q.front();
        II currCoords(y, x);

        Q.pop();

        for (int k = 0; k < 4; ++ k) {
            int newRecursionLevel = recursionLevel;
            II newCoords = currCoords + dirs[k];
            auto [newY, newX] = newCoords;

            if (maze[newY][newX] == PASSAGE && 
                steps[recursionLevel][newY][newX] > steps[recursionLevel][y][x] + 1)
            {
                steps[recursionLevel][newY][newX] = steps[recursionLevel][y][x] + 1;

                if (coordsToPortals.find(newCoords) != coordsToPortals.end())
                {
                    if (portals[coordsToPortals[newCoords]].size() != 2) {
                        if (recursionLevel == 0 &&
                            coordsToPortals[newCoords] == coordsToPortals[target])
                        {
                            return steps[0][newY][newX];
                        }

                        continue;
                    }

                    const std::vector<II>& tmp = portals[coordsToPortals[newCoords]];
                    auto it = std::find(tmp.begin(), tmp.end(), newCoords);

                    if (tmp.begin() == it) {
                        ++ it;
                    } else {
                        -- it;
                    }

                    if (part2) {
                        if (isOuterLayer(newCoords)) {
                            newRecursionLevel --;
                        } else {
                            newRecursionLevel ++;
                        }

                        if (newRecursionLevel < 0 || newRecursionLevel >= (int)portals.size()) {
                            continue;
                        }
                    }

                    newCoords = (*it);
                    steps[newRecursionLevel][newCoords.first][newCoords.second] = steps[recursionLevel][newY][newX] + 1;
                }

                Q.push(III(newCoords.first, newCoords.second, newRecursionLevel));
            }
        }
    }

    return steps[0][target.first][target.second];
}

int main() {
    std::ifstream fin("data.in");
    std::string str;

    while (std::getline(fin, str)) {
        maze.push_back(str);
    }

    fin.close();

    getPortals();

    std::cout << "The answer for part1 is: " << BFS(portals["AA"].back(), portals["ZZ"].back()) << "\n";
    std::cout << "The answer for part2 is: " << BFS(portals["AA"].back(), portals["ZZ"].back(), true) << "\n";

    return 0;
}
