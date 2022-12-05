#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <tuple>
#include <vector>

static const std::regex cratesReg("(?:(?:\\[(\\w)\\]|\\s{3}))\\s?");
static const std::regex instructionReg("move (\\d+) from (\\d+) to (\\d+)");

std::vector<std::string> tokenize(const std::string &str) {
    static const std::string delim = " ";
    std::vector<std::string> words;
    size_t start = 0, end = 0;

    start = str.find_first_not_of(delim, start);

    while (start != std::string::npos) {
        end = str.find_first_of(delim, start);
        words.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delim, end);
    }

    return words;
}

std::string moveCrates(const std::map<std::string, std::string> &initialStacks,
                       const std::vector<std::tuple<int, std::string, std::string>> &reorgProcedure,
                       bool retainOrder=false)
{
    std::map<std::string, std::string> cratesStacks = initialStacks;
    std::string topCrates;

    for (const auto& [cratesCnt, srcStack, destStack] : reorgProcedure) {
        int availableCrateCnt = std::min(cratesCnt, (int)cratesStacks[srcStack].size());
        size_t srcStackRemainingSize = cratesStacks[srcStack].size() - availableCrateCnt;
        std::string cratesToMove = cratesStacks[srcStack].substr(srcStackRemainingSize);

        if (!retainOrder) {
            std::reverse(cratesToMove.begin(), cratesToMove.end());
        }

        cratesStacks[destStack].append(cratesToMove);
        cratesStacks[srcStack].resize(srcStackRemainingSize);
    }

    for (const auto& [_, crates] : cratesStacks) {
        topCrates += crates.back();
    }

    return topCrates;
}

std::string solvePart1(const std::map<std::string, std::string> &initialStacks,
                       const std::vector<std::tuple<int, std::string, std::string>> &reorgProcedure)
{
    return moveCrates(initialStacks, reorgProcedure, false);
}

std::string solvePart2(const std::map<std::string, std::string> &initialStacks,
                       const std::vector<std::tuple<int, std::string, std::string>> &reorgProcedure)
{
    return moveCrates(initialStacks, reorgProcedure, true);
}

int main() {
    std::ifstream fin("data.in");
    std::string line;
    std::map<std::string, std::string> cratesStacks;
    std::vector<std::tuple<int, std::string, std::string>> reorgProcedure;

    while (std::getline(fin, line, '\n')) {
        static std::vector<std::string> tmpStacks;

        if (line.empty()) {
            std::smatch words;
            std::vector<std::string> colsIdexes = tokenize(tmpStacks.back());
            tmpStacks.pop_back();

            for (auto cratesIt = tmpStacks.rbegin(); cratesIt != tmpStacks.rend(); ++ cratesIt) {
                std::string tmpRow = *cratesIt;
                size_t colIdx = 0;

                while (regex_search(tmpRow, words, cratesReg)) {
                    if (words.size() < 2) {
                        std::cerr << "[ERROR] Failed to parse the stack input at row: <"
                                  << *cratesIt << ">\n";
                        std::exit(EINVAL);
                    }

                    if (!words.str(1).empty()) {
                        cratesStacks[colsIdexes[colIdx]].push_back(words.str(1).front());
                    }

                    ++ colIdx;
                    tmpRow = words.suffix();
                }
            }

            break;
        }

        tmpStacks.push_back(line);
    }

    while (std::getline(fin, line, '\n')) {
        std::smatch words;

        if (std::regex_match(line, words, instructionReg)) {
            reorgProcedure.emplace_back(std::stoi(words[1]), words[2], words[3]);
        } else {
            std::cerr << "[ERROR] The current line doesn't match the expected format: <"
                     << line << ">\n";
            std::exit(EINVAL);
        }
    }

    std::cout << "The answer for part1 is: " << solvePart1(cratesStacks, reorgProcedure) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(cratesStacks, reorgProcedure) << "\n";

    fin.close();

    return 0;
}