#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <vector>

static const size_t ELF_GROUP_SIZE = 3;

char findDuplicateItem(const std::string rucksackContent) {
    std::unordered_set<char> firstCompartiment(rucksackContent.begin(),
        rucksackContent.begin() + rucksackContent.size() / 2);

    for (size_t i = rucksackContent.size() / 2; i < rucksackContent.size(); ++ i) {
        if (firstCompartiment.contains(rucksackContent[i])) {
            return rucksackContent[i];
        }
    }

    return '\0';
}

char findBadgeItem(std::vector<std::string> groupRucksacks) {
    std::string intersection;

    if (groupRucksacks.size() < 2) {
        std::cerr << "Invalid group of size " << groupRucksacks.size() << "\n";
        return '\0';
    }

    for (auto &rucksackContent : groupRucksacks) {
        std::sort(rucksackContent.begin(), rucksackContent.end());
    }

    intersection = groupRucksacks.front();

    for (size_t i = 1; i < groupRucksacks.size(); ++ i) {
        std::string new_intersection;

        std::set_intersection(intersection.begin(), intersection.end(),
            groupRucksacks[i].begin(), groupRucksacks[i].end(),
            std::back_inserter(new_intersection));

        intersection = new_intersection;
    }

    return intersection.front();
}

int getItemPriority(char item) {
    if (item >= 'A' && item <= 'Z') {
        return (int)(item - 'A' + 27);
    }

    if (item >= 'a' && item <= 'z') {
        return (int)(item - 'a' + 1);
    }

    std::cerr << "Invalid item: <" << item << ">\n";
    return 0;
}

int solvePart1(const std::vector<std::string> &rucksacksContents) {
    int prioritiesSum = 0;

    for (const auto &rucksackContent : rucksacksContents) {
        prioritiesSum += getItemPriority(findDuplicateItem(rucksackContent));
    }

    return prioritiesSum;
}

int solvePart2(const std::vector<std::string> &rucksacksContents) {
    int badgesPrioSum = 0;

    for (auto it = rucksacksContents.begin(); it != rucksacksContents.end(); std::advance(it, ELF_GROUP_SIZE))
    {
        badgesPrioSum += getItemPriority(
            findBadgeItem(std::vector<std::string>(it, it + ELF_GROUP_SIZE)));
    }

    return badgesPrioSum;
}

int main() {
    std::ifstream fin("data.in");
    std::vector<std::string> rucksacksContents;
    std::string line;

    while (std::getline(fin, line, '\n')) {
        rucksacksContents.push_back(line);
    }

    std::cout << "The answer for part1 is: " << solvePart1(rucksacksContents) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(rucksacksContents) << "\n";

    fin.close();

    return 0;
}