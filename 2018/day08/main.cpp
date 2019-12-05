#include <iostream>
#include <fstream>
#include <vector>

std::vector<int> V;

int part2(int& ind) {
    int s = 0;
    int nr_nodes = V[ind ++];
    int metadata = V[ind ++];
    std::vector<int> childs;

    for (int i = 0; i < nr_nodes; ++ i) {
        childs.push_back(part2(ind));
    }

    for (int i = 0; i < metadata; ++ i) {
        int tmp = V[ind ++];

        if (nr_nodes > 0) {
            if (tmp - 1 < nr_nodes) {
                s += childs[tmp - 1];
            }
        } else {
            s += tmp;
        }
    }

    return s;
}

int part1(int& ind) {
    int s = 0;
    int nr_nodes = V[ind ++];
    int metadata = V[ind ++];

    for (int i = 0; i < nr_nodes; ++ i) {
        s += part1(ind);
    }

    for (int i = 0; i < metadata; ++ i) {
        s += V[ind ++];
    }

    return s;
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    int tmp;

    while (fin >> tmp) {
        V.push_back(tmp);
    }

    int sm = 0;
    int ind = 0;

    while ((size_t)ind < V.size()) {
        sm += part2(ind);
    }

    int ind1 = 0, ind2 = 0;

    std::cout << "The answer for part1 is: " << part1(ind1) <<"\n";
    std::cout << "The answer for part2 is: " << part2(ind2) <<"\n";

    fin.close();

    return 0;
}