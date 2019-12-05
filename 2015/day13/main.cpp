#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#define NMAX       (int)(10)
#define NAME1_POS  (size_t)(0)
#define ACITON_POS (size_t)(2)
#define POINTS_POS (size_t)(3)
#define NAME2_POS  (size_t)(10)
#define INF        (int)(1 << 30)

const std::string LOSE_STR = "lose";

int cost[NMAX][NMAX];

std::vector<std::string> tokenize(const std::string str) {
    static const std::string delim = " .";
    std::vector<std::string> words;
    size_t start = 0, end = 0;

    while (start != std::string::npos) {
        end = str.find_first_of(delim, start);
        words.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delim, end);
    }

    return words;
}

int getNameInd(std::map<std::string, int>& M, const std::string& name) {
    if (M.find(name) == M.end()) {
        M[name] = M.size();
    }

    return M[name];
}

void seatArrangement(std::vector<bool>& vis, int currHappiness, int guestInd, int& sol) {
    bool guestsSettled = true;

    for (const auto& i : vis) {
        if (!i) {
            guestsSettled = false;
            break;
        }
    }
    
    if (guestsSettled) {
        sol = std::max(sol, currHappiness + cost[0][guestInd]);
    } else {
        for (size_t i = 0; i < vis.size(); ++ i) {
            if (!vis[i]) {
                vis[i] = true;
                seatArrangement(vis, currHappiness + cost[i][guestInd], i, sol);
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
        std::vector<std::string> words = tokenize(str);
        int ind1 = getNameInd(M, words[NAME1_POS]);
        int ind2 = getNameInd(M, words[NAME2_POS]);
        int units = std::stoi(words[POINTS_POS]) * ((words[ACITON_POS] == LOSE_STR) ? -1 : 1);
        
        cost[ind1][ind2] += units;
        cost[ind2][ind1] += units;
    }

    std::vector<bool> vis(M.size(), false);
    int res1 = -INF, res2 = -INF;

    vis[0] = true;
    seatArrangement(vis, 0, 0, res1);
    vis.push_back(false);
    seatArrangement(vis, 0, 0, res2);

    std::cout << "The answer for part1 is: " << res1 << "\n";
    std::cout << "The answer for part2 is: " << res2 << "\n";

    fin.close();

    return 0;
}
