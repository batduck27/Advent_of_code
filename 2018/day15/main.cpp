#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>

#define ELF     (char)('E')
#define GOBLIN  (char)('G')
#define WALL    (char)('#')
#define EMPTY   (char)('.')
#define I_LIFE  (int)(200)
#define A_POWER (int)(3)
#define INF     (int)(1 << 30)

enum Direction {UNAVAILABLE = -1, UP, LEFT, RIGHT, DOWN, DIR_MAX};

const int vert[] = {-1, 0, 0, 1};
const int oriz[] = {0, -1, 1, 0};

typedef struct Coord {
    int y, x;
    std::string path;

    Coord() : y(0), x(0), path() {

    }

    Coord(int _y, int _x) : y(_y), x(_x), path() {

    }

    Coord(int _y, int _x, std::string _path) : y(_y), x(_x), path(_path) {

    }
} Coord;

typedef struct Battle {
    std::vector<std::string> map;
    std::map<std::pair<int, int>, int> units;
    int elfs, goblins;

    Battle() : map(), units(), elfs(0), goblins(0) {

    }

    Battle(std::vector<std::string> _map, std::map<std::pair<int, int>, int> _units, int _elfs, int _goblins)
        : map(_map), units(_units), elfs(_elfs), goblins(_goblins) {

    }
} Battle;

char getTargetType(int y, int x, const std::vector<std::string>& map) {
    return (map[y][x] == GOBLIN) ? ELF : GOBLIN;
}

int getNextMove(int startY, int startX, std::vector<std::string> map) {
    char target = getTargetType(startY, startX, map);
    bool moveComplete = false;
    std::string selectedPath;
    std::queue<Coord> Q;
    Q.push(Coord(startY, startX));

    while (!Q.empty() && !moveComplete) {
        Coord c = Q.front();
        Q.pop();

        for (int k = 0; k < DIR_MAX; ++ k) {
            int y = c.y + vert[k];
            int x = c.x + oriz[k];

            if (map[y][x] == target) {
                selectedPath = c.path;
                moveComplete = true;
                break;
            } else if (map[y][x] == EMPTY) {
                Q.push(Coord(y, x, c.path + std::to_string(k)));
                map[y][x] = WALL;
            }
        }
    }

    return (selectedPath.empty()) ? -1 : selectedPath[0] - '0';
}

void unitMove(int& unitY, int& unitX, Battle& B) {
    int move = getNextMove(unitY, unitX, B.map);
    std::pair<int, int> newPos(unitY, unitX);

    if (move != UNAVAILABLE) {
        newPos.first += vert[move];
        newPos.second += oriz[move];

        B.units[newPos] = B.units[std::make_pair(unitY, unitX)];
        std::swap(B.map[newPos.first][newPos.second], B.map[unitY][unitX]);
        B.units.erase(std::make_pair(unitY, unitX));
    
        unitY = newPos.first;
        unitX = newPos.second;
    }
}

void unitAttack(int unitY, int unitX, Battle& B, int elf_power) {
    std::pair<int, int> targetPos(UNAVAILABLE, UNAVAILABLE);
    char target = getTargetType(unitY, unitX, B.map);
    int minLife = INF;

    // get the enemy with the lowest hp
    for (int k = 0; k < DIR_MAX; ++ k) {
        int y = unitY + vert[k];
        int x = unitX + oriz[k];
        std::pair<int, int> newPos = std::make_pair(y, x);

        if (B.map[y][x] == target && B.units[newPos] < minLife) {
            minLife = B.units[newPos];
            targetPos = newPos;
        }
    }

    if (targetPos.first != UNAVAILABLE) {
        B.units[targetPos] -= (target == GOBLIN) ? elf_power : A_POWER;

        if (B.units[targetPos] <= 0) {
            B.units.erase(targetPos);

            if (B.map[targetPos.first][targetPos.second] == GOBLIN) {
                B.goblins --;
            } else {
                B.elfs --;
            }

            B.map[targetPos.first][targetPos.second] = EMPTY;
        }
    }
}

int fight(Battle& B, int elf_power) {
    int round = 0;
    
    while (B.elfs && B.goblins) {
        std::vector<std::pair<int, int>> keys;

        for (const auto& u: B.units) {
            keys.push_back(u.first);
        }

        bool roundCompleted = true;

        for (auto& k : keys) {
            if (B.goblins == 0 || B.elfs == 0) {
                roundCompleted = false;
            }

            // check if the unit is still alive
            if (B.units.find(k) != B.units.end()) {
                // move phase
                unitMove(k.first, k.second, B);

                // attack phase
                unitAttack(k.first, k.second, B, elf_power);
            }
        }

        round += (int)roundCompleted;
    }

    int s = 0;

    for (const auto& x : B.units) {
        s += x.second;
    }

    return round * s;
}

int part1(Battle B) {
    return fight(B, A_POWER);
}

int part2(const Battle& B) {
    int res = 0;

    for (int elf_power = 4; !res; ++ elf_power) {
        Battle tmpB = B;
        int fightRes = fight(tmpB, elf_power);

        if (B.elfs == tmpB.elfs) {
            res = fightRes;
        }
    }

    return res;
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    std::vector<std::string> map;

    while (std::getline(fin, str)) {
        map.push_back(str);
    }

    std::map<std::pair<int, int>, int> units;
    int elfs = 0, goblins = 0;

    for (size_t y = 0; y < map.size(); ++ y) {
        for (size_t x = 0; x < map[y].size(); ++ x) {
            if (map[y][x] == GOBLIN) {
                units[std::make_pair(y, x)] = I_LIFE;
                goblins ++;
            } else if (map[y][x] == ELF) {
                units[std::make_pair(y, x)] = I_LIFE;
                elfs ++;
            } else {
                // do nothing
            }
        }
    }

    std::cout << "The answer for part1 is: " << part1(Battle(map, units, elfs, goblins)) << "\n";
    std::cout << "The answer for part2 is: " << part2(Battle(map, units, elfs, goblins)) << "\n";

    fin.close();
    return 0;
}