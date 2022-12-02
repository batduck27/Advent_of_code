#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

enum Outcomes {
    DEFEAT = 0,
    DRAW,
    WIN,
};

/* Number of shapes used for the rock-paper-scissors game. */
static const size_t SHAPES_NUM = 3;
/* Oponnent's moves are encoded as either 'A', 'B', or 'C', corresponding to rock, paper,
 * and scissors.
 */
static const char OPONENT_START_SHAPE = 'A';
/* For part 1, our moves are encoded as either 'X', 'Y', 'Z', corresponding to rock, paper,
 * and scissors. For part 2, this is used for representing the outcome of a round, corresponding
 * to a defeat, draw, or win.
 */
static const char OWN_START_SHAPE = 'X';
/* Scores for each shape that can be played by us, in this order: rock, paper, scissors. */
static const int SHAPE_SCORE[SHAPES_NUM] = {1, 2, 3};
/* Scores obtained depending on the outcome of the round, in this order: defeat, draw, win. */
static const int OUTCOME_SCORES[SHAPES_NUM] = {0, 3, 6};

int getRoundOutcome(const int normOponentMove, const int normOwnMove) {
    if (normOwnMove == normOponentMove) {
        return OUTCOME_SCORES[DRAW];
    }

    if (((normOponentMove + 1) % SHAPES_NUM) == normOwnMove) {
        return OUTCOME_SCORES[WIN];
    }

    return OUTCOME_SCORES[DEFEAT];
}

int solvePart1(const std::vector<std::pair<char, char>> &strategyGuide) {
    int finalScore = 0;

    for (const auto& [openentMove, ownMove] : strategyGuide) {
        int normOponentMove = openentMove - OPONENT_START_SHAPE;
        int normOwnMove = ownMove - OWN_START_SHAPE;

        finalScore += getRoundOutcome(normOponentMove, normOwnMove) + SHAPE_SCORE[normOwnMove];
    }

    return finalScore;
}

int solvePart2(const std::vector<std::pair<char, char>> &strategyGuide) {
    int finalScore = 0;

    for (const auto& [openentMove, roundOutcome] : strategyGuide) {
        int normOponentMove = openentMove - OPONENT_START_SHAPE;
        int normRoundOutcome = roundOutcome - OWN_START_SHAPE;
        int normOwnMove = SHAPES_NUM;

        switch (normRoundOutcome) {
            case DEFEAT:
                normOwnMove = (normOponentMove + SHAPES_NUM - 1) % SHAPES_NUM; 
                break;
            case DRAW:
                normOwnMove = normOponentMove;
                break;
            case WIN:
                normOwnMove = (normOponentMove + 1) % SHAPES_NUM;
                break;
            default:
                std::cerr << "Unknown round outcome: " << roundOutcome << "\n";
                std::exit(EINVAL);
        }

        finalScore += OUTCOME_SCORES[normRoundOutcome] + SHAPE_SCORE[normOwnMove];
    }


    return finalScore;
}

int main() {
    std::ifstream fin("data.in");
    std::vector<std::pair<char, char>> strategyGuide;
    std::string line;

    while (std::getline(fin, line, '\n')) {
        strategyGuide.emplace_back(line[0], line[2]);
    }

    std::cout << "The answer for part1 is: " << solvePart1(strategyGuide) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(strategyGuide) << "\n";

    fin.close();

    return 0;
}