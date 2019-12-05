#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#include <queue>

#include <stdio.h>

#define NMAX 26
#define WORKERS 5
#define SECONDS 61

std::vector<char> graph[NMAX];
std::vector<char> tGraph[NMAX];

// check if @node cis available (has all the parents completed)
bool isAvailable(const std::vector<bool>& vis, char node) {
    for (const auto& x : tGraph[node - 'A']) {
        if (!vis[x - 'A']) {
            return false;
        }
    }

    return true;
}

// get the roots of the graph - the vertices which are available from the start (gr[node] == 0)
std::vector<char> getRoots(std::vector<bool> vis, std::vector<int> gr) {
    std::vector<char> tmp;

    for (int i = 0; i < NMAX; ++ i) {
        if (!vis[i] && gr[i] == 0) {
            tmp.push_back(i + 'A');
        }
    }

    return tmp;
}

std::string part1(std::vector<char> roots, std::vector<bool> vis) {
    std::vector<char> V;
    std::set<char> Q;

    for (const auto& x : roots) {
        Q.insert(x);
    }

    while (Q.size()) {
        char node = *Q.begin();

        vis[node - 'A'] = true;
        V.push_back(node);
        Q.erase(Q.begin());

        for (const auto& x : graph[node - 'A']) {
            if (!vis[x - 'A'] && isAvailable(vis, x)) {
                Q.insert(x);
            }
        }
    }

    return std::string(V.begin(), V.end());
}

int part2(std::vector<char> roots, std::vector<bool> vis) {
    std::queue<char> Q;
    std::set<std::pair<int, char>> S;
    int res = 0;

    for (const auto& x : roots) {
        Q.push(x);
    }

    while (!Q.empty() || !S.empty()) {
        while (S.size() < WORKERS && !Q.empty()) {
            char node = Q.front();
            Q.pop();
            S.insert(std::make_pair(node - 'A' + SECONDS, node));
        }

        std::vector<std::pair<int, char>> tmp(S.begin(), S.end());
        int timePassed = (*S.begin()).first;
        int first = 0;

        res += timePassed;

        for (size_t i = 0; i < tmp.size(); ++ i) {
            tmp[i].first -= timePassed;
        
            if (tmp[i].first == 0) {
                first = i + 1;
                vis[tmp[i].second - 'A'] = true;

                for (const auto& x : graph[tmp[i].second - 'A']) {
                    if (!vis[x - 'A'] && isAvailable(vis, x)) {
                        Q.push(x);
                    }
                }
            }
        }

        S.clear();
        S.insert(tmp.begin() + first, tmp.end());
    }

    return res;
}

int main() {
    std::vector<bool> vis(NMAX, true);
    std::vector<int> gr(NMAX);
    
    std::ifstream fin("data.in");
    std::string str;
    
    while (std::getline(fin, str)) {
        char a, b;

        sscanf(str.c_str(), "Step %c must be finished before step %c can begin.", &a, &b);

        graph[a - 'A'].push_back(b);
        tGraph[b - 'A'].push_back(a);
        vis[a - 'A'] = vis[b - 'A'] = false;
        gr[b - 'A'] ++;
    }

    std::cout << "The answer for part1 is: " << part1(getRoots(vis, gr), vis) << "\n";
    std::cout << "The answer for part2 is: " << part2(getRoots(vis, gr), vis) << "\n";

    fin.close();

    return 0;
}