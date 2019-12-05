#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iterator>

int getIndex(std::vector<int> V)
{
    int ind, max = 0;

    for (size_t i = 0; i < V.size(); ++ i)
        if (V[i] > max) {
            max = V[i];
            ind = (int)i;
        }

    return ind;
}

bool doCycle(std::vector<int>& V, std::map<std::vector<int>, int>& M, int& steps)
{
    int ind = getIndex(V);
    int nr = V[ind];

    ++ steps;
    V[ind] = 0;

    for (size_t i = (ind + 1) % V.size(); nr; --nr, i = (i + 1) % V.size())
        V[i] ++;

    if (M.find(V) == M.end())
        M.insert(std::make_pair(V, steps));
    else
        return false;

    return true;
}

int main()
{
    std::ifstream fin("data.in");
    std::vector<int> V {std::istream_iterator<int>{fin}, {}};
    std::map <std::vector<int>, int> M;
    int steps = 0;

    M.insert(std::make_pair(V, steps));

    fin.close();

    while (doCycle(V, M, steps));

    std::cout << "The answer for part1 is: " << steps << "\n";
    std::cout << "The answer for part2 is: " << steps - M[V] << "\n";

    return 0;
}