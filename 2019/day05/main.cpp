#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <map>

static const int INPUT_PART1 = 1;
static const int INPUT_PART2 = 5;

enum OPCODE {
    ADD  = 1,
    MUL  = 2,
    IN   = 3,
    OUT  = 4,
    JMPT = 5,
    JMPF = 6,
    LT   = 7,
    EQ   = 8,
    HALT = 99,
};

enum PARAM_MODE {
    POSITION = 0,
    IMMEDIATE = 1,
};

static const std::map<enum OPCODE, int> opSizes{
    { ADD,  4 },
    { MUL,  4 },
    { IN,   2 },
    { OUT,  2 },
    { JMPT, 3 },
    { JMPF, 3 },
    { LT,   4 },
    { EQ,   4 },
    { HALT, 1 },
};

typedef struct Instruction {
    int opcode;
    int param1, param2, param3;
    enum PARAM_MODE mode1, mode2, mode3;
} Instruction;

class IntCodeComputer {
private:
    std::vector<int> v;
    int ip; // instruction pointer
    int diagCode;

    Instruction getNextInstruction();
    int getValueByMode(int param, enum PARAM_MODE mode);

    void addInstruction(Instruction ins);
    void mulInstruction(Instruction ins);
    void inInstruction(Instruction ins, int input);
    void outInstruction(Instruction ins);
    void jmptInstruction(Instruction ins);
    void jmpfInstruction(Instruction ins);
    void ltInstruction(Instruction ins);
    void eqInstruction(Instruction ins);

public:
    IntCodeComputer() : v(), ip(), diagCode() {

    }

    IntCodeComputer(std::vector<int> v) : v(v), ip(), diagCode() {

    }

    int run(int input);
};

int main() {
    std::ifstream fin("data.in");
    std::vector<int> v;
    int x;

    while (fin >> x) {
        v.push_back(x);
        fin.ignore();
    }

    fin.close();

    IntCodeComputer computer{v};

    std::cout << "The answer for part1 is: " << computer.run(INPUT_PART1) << "\n";
    std::cout << "The answer for part2 is: " << computer.run(INPUT_PART2) << "\n";

    return 0;
}

int IntCodeComputer::getValueByMode(int param, enum PARAM_MODE mode) {
    if (mode == POSITION) {
        return v[param];
    }

    return param;
}

int IntCodeComputer::run(int input) {
    std::vector<int> copyV = v;

    ip       = 0;
    diagCode = 0;

    while (ip >= 0 && ip < (int)v.size()) {
        Instruction ins = getNextInstruction();

        switch (ins.opcode) {
        case ADD:
            addInstruction(ins);
            break;
        case MUL:
            mulInstruction(ins);
            break;
        case IN:
            inInstruction(ins, input);
            break;
        case OUT:
            outInstruction(ins);
            break;
        case JMPT:
            jmptInstruction(ins);
            break;
        case JMPF:
            jmpfInstruction(ins);
            break;
        case LT:
            ltInstruction(ins);
            break;
        case EQ:
            eqInstruction(ins);
            break;
        case HALT:
            ip = v.size();
            break;
        default:
            std::cout << "Invalid opcode " << ins.opcode << " at index " << ip << "\n";
            ip = -1;
            break;
        }
    }

    v  = copyV;
    ip = 0;

    return diagCode;
}

Instruction IntCodeComputer::getNextInstruction() {
    Instruction ins = {0};

    ins.opcode = v[ip] % 100;
    ins.mode1  = (enum PARAM_MODE)((v[ip] / 100) % 10);
    ins.mode2  = (enum PARAM_MODE)((v[ip] / 1000) % 10);
    ins.mode3  = (enum PARAM_MODE)((v[ip] / 10000) % 10);

    if (ins.opcode == HALT) {
        return ins;
    }

    ins.param1 = v[ip + 1];

    if (ins.opcode == IN || ins.opcode == OUT) {
        return ins;
    }

    ins.param2 = v[ip + 2];

    if (ins.opcode == JMPT || ins.opcode == JMPF) {
        return ins;
    }

    ins.param3 = v[ip + 3];

    return ins;
}

void IntCodeComputer::addInstruction(Instruction ins) {
    v[ins.param3] = getValueByMode(ins.param1, ins.mode1) +
                    getValueByMode(ins.param2, ins.mode2);
    ip += opSizes.at(ADD);
}

void IntCodeComputer::mulInstruction(Instruction ins) {
    v[ins.param3] = getValueByMode(ins.param1, ins.mode1) *
                    getValueByMode(ins.param2, ins.mode2);
    ip += opSizes.at(MUL);
}

void IntCodeComputer::inInstruction(Instruction ins, int input) {
    v[ins.param1] = input;
    ip += opSizes.at(IN);
}

void IntCodeComputer::outInstruction(Instruction ins) {
    diagCode = getValueByMode(ins.param1, ins.mode1);
    ip += opSizes.at(OUT);
}

void IntCodeComputer::jmptInstruction(Instruction ins) {
    if (getValueByMode(ins.param1, ins.mode1) != 0) {
        ip = getValueByMode(ins.param2, ins.mode2);
    } else {
        ip += opSizes.at(JMPT);
    }
}

void IntCodeComputer::jmpfInstruction(Instruction ins) {
    if (getValueByMode(ins.param1, ins.mode1) == 0) {
        ip = getValueByMode(ins.param2, ins.mode2);
    } else {
        ip += opSizes.at(JMPF);
    }
}

void IntCodeComputer::ltInstruction(Instruction ins) {
    int diff = 0;

    diff = getValueByMode(ins.param1, ins.mode1) -
           getValueByMode(ins.param2, ins.mode2);

    v[ins.param3] = (diff < 0);
    ip += opSizes.at(LT);
}

void IntCodeComputer::eqInstruction(Instruction ins) {
    int diff = 0;

    diff = getValueByMode(ins.param1, ins.mode1) -
           getValueByMode(ins.param2, ins.mode2);

    v[ins.param3] = (diff == 0);
    ip += opSizes.at(EQ);
}
