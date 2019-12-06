#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <queue>
#include <vector>

std::map<std::string, std::vector<std::string>> G;

static std::vector<std::string> tokenize(const std::string str) {
    static const std::string delim = " )";
    std::vector<std::string> words;
    size_t start = 0, end = 0;

    while (start != std::string::npos) {
        end = str.find_first_of(delim, start);
        words.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delim, end);
    }

    return words;
}

int dfs(const std::string& currNode, std::set<std::string>& vis, int level = 0) {
    int s = 0;

    vis.insert(currNode);

    for (const auto &nextNode : G[currNode]) {
        if (vis.find(nextNode) == vis.end()) {
            s += dfs(nextNode, vis, level + 1);
        }
    }

    return s + level;
}

int bfs(std::string start, std::string target) {
    std::queue<std::string> Q;
    std::map<std::string, int> vis;

    Q.push(start);
    vis[start] = 0;

    while (!Q.empty()) {
        std::string currNode = Q.front();

        Q.pop();

        for (const auto &nextNode : G[currNode]) {
            if (vis.find(nextNode) == vis.end()) {
                Q.push(nextNode);
                vis[nextNode] = vis[currNode] + 1;
            }
        }

        if (vis.find(target) != vis.end()) {
            return vis[target] - 2;
        }
    }

    return vis[target] - 2;
}

int main() {
    std::ifstream fin("data.in");
    std::set<std::string> dfsVis;
    std::string line;

    while (std::getline(fin, line)) {
        std::vector<std::string> objects = tokenize(line);

        if (objects.size() != 2) {
            break;
        }

        G[objects[0]].push_back(objects[1]);
        G[objects[1]].push_back(objects[0]);
    }

    fin.close();

    std::cout << "The answer for part 1 is: " << dfs("COM", dfsVis) << "\n";
    std::cout << "The answer for part 2 is: " << bfs("YOU", "SAN") << "\n";

    return 0;
}
