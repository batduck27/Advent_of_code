#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <regex>
#include <limits>

static const std::string BASE_REACTIVE = "ORE";
static const std::string FINAL_REACTIVE = "FUEL";
static const long long PART2_QUANTITY = 1000000000000LL;

typedef std::pair<long long, std::string> Reactive;
typedef std::pair<long long, std::vector<Reactive>> NeededReactives; 

std::map<std::string, NeededReactives> reactions;
std::map<std::string, long long> leftovers;

void decompose(const std::string& currReactive, long long needed, long long& total) {
    if (currReactive == BASE_REACTIVE) {
        // we reached the "ORE" reactive, no other reactions from here for this branch
        total += needed;
        return;
    }

    // check if there are any leftovers from the other reactions
    if (leftovers[currReactive] >= needed) {
        // the leftovers exceeds the needed quantity of this reactive
        leftovers[currReactive] -= needed;
        return;
    }

    auto [currReactiveCnt, components] = reactions[currReactive];
    long long factor = 1;

    // substract the leftovers (produce only how much is needed)
    needed -= leftovers[currReactive];
    leftovers[currReactive] = 0;

    // we need more reactions to produce the needed quantity, get the number of reactions
    if (needed > currReactiveCnt) {
        factor = needed / currReactiveCnt;

        while (factor * currReactiveCnt < needed) {
            factor ++;
        }
    }

    // save the number of leftovers of this reactive
    leftovers[currReactive] = factor * currReactiveCnt - needed;

    // take every reactive from the reaction and obtain it
    for (const auto& c : components) {
        auto [reactiveCnt, reactive] = c;

        decompose(reactive, factor * reactiveCnt, total);
    }
}

long long decomposeWrapper(const std::string& currReactive, long long needed) {
    long long total = 0;

    leftovers.clear();
    decompose(currReactive, needed, total);

    return total;
}

long long solvePart2() {
    const long long maxSteps = std::numeric_limits<int>::max();
    long long quantity = 0;

    for (long long step = maxSteps; step; step >>= 1) {
        if (decomposeWrapper(FINAL_REACTIVE, step + quantity) <= PART2_QUANTITY) {
            quantity += step;
        }
    }

    return quantity;
}

int main() {
    const std::regex chemicalRegex("(\\d+) (\\w+)");
    std::ifstream fin("data.in");
    std::string line;

    while (std::getline(fin, line) && !line.empty()) {
        auto it_begin = std::sregex_iterator(line.begin(), line.end(), chemicalRegex);
        auto it_end   = std::sregex_iterator();
        std::vector<std::pair<long long, std::string>> tmp;
        int matches = std::distance(it_begin, it_end);
        std::smatch w = *it_begin;

        for (int i = 0; i < matches - 1; ++ i) {
            tmp.push_back(std::make_pair(std::stoi(w[1]), w[2]));
            w = *(++ it_begin);
        }

        reactions[w[2]] = std::make_pair(std::stoi(w[1]), tmp);
    }

    fin.close();

    std::cout << "The answer for part1 is: " << decomposeWrapper(FINAL_REACTIVE, 1LL) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2() << "\n";

    return 0;
}