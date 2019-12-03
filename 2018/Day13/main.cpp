#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

#define DIR_LAST 4
#define I_LAST 3

enum Direction {UP, RIGHT, DOWN, LEFT};
enum Intersection {I_LEFT, I_STRAIGHT, I_RIGHT};

static const int vert[] = {-1, 0, 1, 0};
static const int oriz[] = {0, 1, 0, -1};

typedef struct Cart {
    int dir;
    int turn;
    bool destroyed;

    Cart(int _dir) : turn(I_LEFT), destroyed(false) {
        dir = _dir;
    }

    Cart() : dir(0), turn(I_LEFT), destroyed(false) {

    }
} Cart;

std::map<std::pair<int, int>, Cart> M;
std::vector<std::string> V;
std::vector<Cart> carts;

void moveCart(int& y, int& x, Cart& c)
{

    switch (V[y][x]) {
        case '/':
            switch (c.dir) {
                case UP:
                    c.dir = RIGHT;
                    break;
                case RIGHT:
                    c.dir = UP;
                    break;
                case DOWN:
                    c.dir = LEFT;
                    break;
                case LEFT:
                    c.dir = DOWN;
                    break;
                default:
                    break; 
            }
            break;
        case '\\':
            switch (c.dir) {
                case UP:
                    c.dir = LEFT;
                    break;
                case RIGHT:
                    c.dir = DOWN;
                    break;
                case DOWN:
                    c.dir = RIGHT;
                    break;
                case LEFT:
                    c.dir = UP;
                    break;
                default:
                    break; 
            }
            break;
        case '+':
            c.dir = ((c.dir + (c.turn - 1)) + DIR_LAST) % DIR_LAST;
            c.turn = (c.turn + 1) % I_LAST;
            break;
        default:
            break;
    }
    
    y += vert[c.dir];
    x += oriz[c.dir];
}

int main() {
    std::ifstream fin("data.in");
    std::string str;

    while (std::getline(fin, str)) {
        V.push_back(str);
    }

    for (size_t y = 0; y < V.size(); ++ y) {
        for (size_t x = 0; x < V[y].size(); ++ x) {
            switch (V[y][x]) {
                case '>':
                    M[std::make_pair(y, x)] = Cart(RIGHT);
                    V[y][x] = '-';
                    break;
                case '<':
                    M[std::make_pair(y, x)] = Cart(LEFT);
                    V[y][x] = '-';
                    break;
                case 'v':
                    M[std::make_pair(y, x)] = Cart(DOWN);
                    V[y][x] = '|';
                    break;
                case '^':
                    M[std::make_pair(y, x)] = Cart(UP);
                    V[y][x] = '|';
                    break;
                default:
                    break;
            }
        }
    }

    bool part1Solved = false;

    while (M.size() > 1) {
        std::vector<std::pair<int, int>> keys;

        for (const auto& x : M) {
            keys.push_back(x.first);
        }

        for (const auto& x : keys) {
            if (M.find(x) != M.end()) {
                std::pair<int, int> pos = x;
                Cart c = M[pos];

                M.erase(x);

                moveCart(pos.first, pos.second, c);

                if (M.find(pos) != M.end()) {
                    M.erase(pos);

                    if (!part1Solved) {
                        std::cout << "The answer for part1 is: (" << pos.second << "," << pos.first << ")\n";
                        part1Solved = true; 
                    }
                } else {
                    M[pos] = c;
                }
            }
        }
    }

    std::cout << "The answer for part2 is: (" << M.begin()->first.second << "," << M.begin()->first.first <<")\n";

    fin.close();

    return 0;
}