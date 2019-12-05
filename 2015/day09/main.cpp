#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <stdio.h>

#define NMAX      (int)(15)
#define NAME_SIZE (int)(25)
#define INF       (int)(1 << 30)

const std::string frmt = "%s to %s = %d";

int cost[NMAX][NMAX];
int res1 = INF, res2 = -INF;

int getNameInd(std::map<std::string, int>& M, const std::string& name) {
    if (M.find(name) == M.end()) {
        M[name] = M.size();
    }

    return M[name];
}

void generatePath(std::vector<bool>& vis, int currDist, int lastCity) {
    bool citiesVisited = true;

    for (const auto& i : vis) {
        if (!i) {
            citiesVisited = false;
            break;
        }
    }
    
    if (citiesVisited) {
        res2 = std::max(res2, currDist);
        res1 = std::min(res1, currDist);
    } else {
        for (size_t i = 0; i < vis.size(); ++ i) {
            if (!vis[i]) {
                vis[i] = true;
                generatePath(vis, currDist + cost[i][lastCity], i);
                vis[i] = false;
            }
        }
    }
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    std::map<std::string, int> M;

    while (std::getline(fin, str)) {
        char from[NAME_SIZE], to[NAME_SIZE];
        int dist;

        sscanf(str.c_str(), frmt.c_str(), from, to, &dist);

        int ind1 = getNameInd(M, std::string(from));
        int ind2 = getNameInd(M, std::string(to));
                
        cost[ind1][ind2] = cost[ind2][ind1] = dist;
    }

    std::vector<bool> vis(M.size(), false);

    for (size_t i = 0; i < vis.size(); ++ i) {
        vis[i] = true;
        generatePath(vis, 0, i);
        vis[i] = false;
    }
  
    std::cout << "The answer for part1 is: " << res1 << "\n";
    std::cout << "The answer for part2 is: " << res2 << "\n";

    fin.close();

    return 0;
}
