#include <iostream>
#include <fstream>
#include <vector>

static const int IND1_VAL = 12;
static const int IND2_VAL = 2;
static const int MAX_OPCODE = 100;
static const int REQUIRED_OUT = 19690720;

enum OPCODE {
    ADD = 1,
    MUL = 2,
    HALT = 99,
};

int runCode(std::vector<int> v) {
    for (int i = 0; i < (int)v.size(); i += 4) {
        int ind1, ind2, ind3;

        if (v[i] == HALT) {
            break;
        }

        ind1 = v[i + 1];
        ind2 = v[i + 2];
        ind3 = v[i + 3];

        if (v[i] == ADD) {
            v[ind3] = v[ind1] + v[ind2];
        } else if (v[i] == MUL) {
            v[ind3] = v[ind1] * v[ind2];
        } else {
            std::cerr << "Invalid opcode: " << v[i] << "\n";
        }
    }

    return v[0];
}

int solvePart1(std::vector<int> v) {
    v[1] = IND1_VAL;
    v[2] = IND2_VAL;

    return runCode(v);
}

int solvePart2(std::vector<int> v) {
    for (int noun = 0; noun < MAX_OPCODE; ++ noun) {
        for (int verb = 0; verb < MAX_OPCODE; ++ verb) {
            int res;

            v[1] = noun;
            v[2] = verb;

            res = runCode(v);
            if (res == REQUIRED_OUT) {
                return noun * 100 + verb;
            }
        }
    }

    return -1;
}

int main() {
    std::ifstream fin("data.in");
    std::vector<int> v;
    int x;

    while (fin >> x) {
        char ch;
        v.push_back(x);
        fin >> ch;
    }

    fin.close();

    std::cout << "The answer for part1 is: " << solvePart1(v) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(v) << "\n";

    return 0;
}
