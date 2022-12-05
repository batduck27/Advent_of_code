#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

typedef std::pair<int, int> II;
static const std::regex pairsReg("(\\d+)-(\\d+),(\\d+)-(\\d+)");

bool containsSection(const II &section1, const II &section2) {
    if (section1.first < section2.first || section1.first > section2.second) {
        return false;
    }

    if (section1.second < section2.first || section1.second > section2.second) {
        return false;
    }

    return true;
}

bool isOverlapping(const II &section1, const II &section2) {
    if (section1.first >= section2.first && section1.first <= section2.second) {
        return true;
    }

    if (section1.second >= section2.first && section1.second <= section2.second) {
        return true;
    }

    return false;
}

int solvePart1(const std::vector<std::pair<II, II>> &assignedSections) {
    int containedSectionsCnt = 0;

    for (const auto& [elf1Section, elf2Section] : assignedSections) {
        containedSectionsCnt += (containsSection(elf1Section, elf2Section) ||
                                 containsSection(elf2Section, elf1Section));
    }

    return containedSectionsCnt;
}

int solvePart2(const std::vector<std::pair<II, II>> &assignedSections) {
    int overlappingPairsCnt = 0;

    for (const auto& [elf1Section, elf2Section] : assignedSections) {
        overlappingPairsCnt += (isOverlapping(elf1Section, elf2Section) ||
                                isOverlapping(elf2Section, elf1Section));
    }

    return overlappingPairsCnt;
}

int main() {
    std::ifstream fin("data.in");
    std::vector<std::pair<II, II>> assignedSections;
    std::string line;

    while (std::getline(fin, line, '\n')) {
        std::smatch words;

        if (std::regex_match(line, words, pairsReg)) {
            assignedSections.emplace_back(II(std::stoi(words[1]), std::stoi(words[2])),
                                          II(std::stoi(words[3]), std::stoi(words[4])));
        } else {
            std::cerr << "[ERROR] The current line doesn't match the expected format: <"
                     << line << ">\n";
            std::exit(EINVAL);
        }
    }

    std::cout << "The answer for part1 is: " << solvePart1(assignedSections) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(assignedSections) << "\n";

    fin.close();

    return 0;
}