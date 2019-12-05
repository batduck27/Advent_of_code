#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>

static const int vert[] = {-1, 0, 1, 0};
static const int oriz[] = {0, 1, 0, -1};
static const char INFECTED = '#';
static const char FLAGGED = 'F';
static const char WEAKENED = 'W';

int solveTask1(std::map<std::pair<int, int>, char> M, int x, int y)
{
    static const int STEPS = 10000;
    int dir = 0; // UP
    int infections = 0;

    for (int i = 0; i < STEPS; ++ i) {
        std::pair<int, int> coords = std::make_pair(y, x);

        // check if the node is infected
        if (M.find(coords) != M.end()) {
            // clean the node
            M.erase(coords);
            
            // turn right
            dir = (dir + 1) % 4;
        } else {
            // node is clean, infect it
            M[coords] = INFECTED;

            // increment infections counter
            ++ infections;

            // turn left
            dir = (4 + dir - 1) % 4;
        }

        // move
        y += vert[dir];
        x += oriz[dir];
    }

    return infections;
}

int solveTask2(std::map<std::pair<int, int>, char> M, int x, int y)
{
    static const int STEPS = 10000000;
    int dir = 0; // UP
    int infections = 0;

    for (int i = 0; i < STEPS; ++ i) {
        std::pair<int, int> coords = std::make_pair(y, x);

        //  the node is clean
        if (M.find(coords) == M.end()) {
            // weaken it
            M[coords] = WEAKENED;

            // turn left
            dir = (4 + dir - 1) % 4;
        } else {
            // node is infected / weakened / flagged
            if (M[coords] == WEAKENED) {
                // the node is weakened, infect it    
                M[coords] = INFECTED;

                // incremend infections counter
                ++ infections;
            } else if (M[coords] == INFECTED) {
                // the node is infected, flag it
                M[coords] = FLAGGED;
                
                // turn right
                dir = (dir + 1) % 4;
            } else {
                // the node is flagged, clear it
                M.erase(coords);

                // reverse direction
                dir = (dir + 2) % 4;
            }
        }

        // move
        y += vert[dir];
        x += oriz[dir];
    }

    return infections;
}

int main()
{
    std::ifstream fin("data.in");
    std::map<std::pair<int, int>, char> M;
    std::string line;
    int nr;

    for (nr = 0; std::getline(fin, line); ++ nr) {
        for (int j = 0; j < (int)line.size() ; ++ j) {
            if (line[j] == '#') {
                M[std::make_pair(nr, j)] = INFECTED;
            }
        }
    }

    fin.close();

    std::cout << "The answer for part1 is: " << solveTask1(M, nr / 2, nr / 2) << "\n";
    std::cout << "The answer for part2 is: " << solveTask2(M, nr / 2, nr / 2) << "\n";

    return 0;
}