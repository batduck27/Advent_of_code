#include <iostream>
#include <fstream>
#include <vector>

#define NAME_SIZE (int)(20)

const std::string fmt = "%s can fly %d km/s for %d seconds, but then must rest for %d seconds.";
const int finishTime = 2503;
const int INF = (1 << 30);

typedef struct Reindeer {
    int speed, time, rest;
    bool moving;
    int distance, timePassed, points;

    Reindeer() : speed(), time(), rest(), moving(true), distance(0), timePassed(0), points(0) {

    }
} Reindeer;

int solvePart1(const std::vector<Reindeer>& Reins) {
    int maxDist = -INF;

    for (const auto& r : Reins) {
        int aux = finishTime / (r.time + r.rest);
        int t = finishTime - aux * (r.time + r.rest);

        maxDist = std::max(maxDist, aux * r.speed * r.time + std::min(t, r.time) * r.speed);
    }

    return maxDist;
}

int solvePart2(std::vector<Reindeer> Reins) {
    int maxPoints = -INF;

    for (int t = 0; t < finishTime; ++ t) {
        int currMaxDist = -INF;

        // update the stats for deers
        for (auto& r : Reins) {
            ++ r.timePassed;

            if (r.moving) {
                r.distance += r.speed;

                if (r.timePassed == r.time) {
                    r.moving = false;
                    r.timePassed = 0;
                }
            } else if (r.timePassed == r.rest) {
                r.moving = true;
                r.timePassed = 0;
            } else {
                // do nothing
            }

            currMaxDist = std::max(currMaxDist, r.distance);
        }

        // now award the points to the leading deer(s)
        for (auto& r: Reins) {
            if (r.distance == currMaxDist) {
                ++ r.points;
                maxPoints = std::max(maxPoints, r.points);
            }
        }
    }

    return maxPoints;
}

int main()
{
    std::ifstream fin("data.in");
    std::string str;
    std::vector<Reindeer> R;

    while (std::getline(fin, str)) {
        char name[NAME_SIZE];
        Reindeer r;

        sscanf(str.c_str(), fmt.c_str(), name, &r.speed, &r.time, &r.rest);
    
        R.push_back(r);
    }

    std::cout << "The answer for part1 is: " << solvePart1(R) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(R) << "\n";

    fin.close();

    return 0;
}
