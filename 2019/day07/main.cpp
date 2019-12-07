#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <numeric>

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

static const int INF = (1 << 30);
static const int AMPS_NO = 5;
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

class Amplifier {
private:
    std::queue<int> input;
    std::vector<int> v;
    std::vector<int> copyV;
    int ip; // instruction pointer
    int diagCode;

    Instruction getNextInstruction();
    int getValueByMode(int param, enum PARAM_MODE mode);

    void addInstruction(Instruction ins);
    void mulInstruction(Instruction ins);
    void inInstruction(Instruction ins);
    void outInstruction(Instruction ins);
    void jmptInstruction(Instruction ins);
    void jmpfInstruction(Instruction ins);
    void ltInstruction(Instruction ins);
    void eqInstruction(Instruction ins);

public:
    Amplifier() : input(), v(), copyV(), ip(), diagCode() {
    }

    Amplifier(std::vector<int> v) : input(), v(v), copyV(v), ip(), diagCode() {
    }

    bool hasHalted();
    int run();
    void reset();
    void provideInput(int);
};

int getMaxOutput(std::vector<Amplifier> amps, int offset = 0) {
    int phase[amps.size()];
    int maxOutput = -INF;

    std::iota(phase, phase + amps.size(), offset);

    do {
        int ampOutput = 0;
        int lastAmp = amps.size() - 1;
        int i = 0;

        for (int j = 0; j < (int)amps.size(); ++ j) {
            amps[j].reset();
            amps[j].provideInput(phase[j]);
        }

        while (!amps[lastAmp].hasHalted()) {
            amps[i].provideInput(ampOutput);
            ampOutput = amps[i].run();
            i = (i + 1) % amps.size();
        }

        maxOutput = std::max(maxOutput, ampOutput);
    } while (std::next_permutation(phase, phase + amps.size()));

    return maxOutput;
}

int main() {
    std::ifstream fin("data.in");
    std::vector<int> v;
    std::vector<Amplifier> amps;
    int x;

    while (fin >> x) {
        v.push_back(x);
        fin.ignore();
    }

    fin.close();

    for (int i = 0; i < AMPS_NO; ++ i) {
        amps.push_back(Amplifier{v});
    }

    std::cout << "The answer for part1 is: " << getMaxOutput(amps) << "\n";
    std::cout << "The answer for part2 is: " << getMaxOutput(amps, 5) << "\n";

    return 0;
}

int Amplifier::getValueByMode(int param, enum PARAM_MODE mode) {
    if (mode == POSITION) {
        return v[param];
    }

    return param;
}

void Amplifier::provideInput(int in) {
    input.push(in);
}

void Amplifier::reset() {
    ip       = 0;
    diagCode = 0;
    v        = copyV;

    while (!input.empty()) {
        input.pop();
    }
} 

bool Amplifier::hasHalted() {
    return (ip == v.size());
}

int Amplifier::run() {
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
            if (input.empty()) {
                return diagCode;
            }
            inInstruction(ins);
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

    return diagCode;
}

Instruction Amplifier::getNextInstruction() {
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

void Amplifier::addInstruction(Instruction ins) {
    v[ins.param3] = getValueByMode(ins.param1, ins.mode1) +
                    getValueByMode(ins.param2, ins.mode2);
    ip += opSizes.at(ADD);
}

void Amplifier::mulInstruction(Instruction ins) {
    v[ins.param3] = getValueByMode(ins.param1, ins.mode1) *
                    getValueByMode(ins.param2, ins.mode2);
    ip += opSizes.at(MUL);
}

void Amplifier::inInstruction(Instruction ins) {
    v[ins.param1] = input.front();
    input.pop();
    ip += opSizes.at(IN);
}

void Amplifier::outInstruction(Instruction ins) {
    diagCode = getValueByMode(ins.param1, ins.mode1);
    ip += opSizes.at(OUT);
}

void Amplifier::jmptInstruction(Instruction ins) {
    if (getValueByMode(ins.param1, ins.mode1) != 0) {
        ip = getValueByMode(ins.param2, ins.mode2);
    } else {
        ip += opSizes.at(JMPT);
    }
}

void Amplifier::jmpfInstruction(Instruction ins) {
    if (getValueByMode(ins.param1, ins.mode1) == 0) {
        ip = getValueByMode(ins.param2, ins.mode2);
    } else {
        ip += opSizes.at(JMPF);
    }
}

void Amplifier::ltInstruction(Instruction ins) {
    int diff = 0;

    diff = getValueByMode(ins.param1, ins.mode1) -
           getValueByMode(ins.param2, ins.mode2);

    v[ins.param3] = (diff < 0);
    ip += opSizes.at(LT);
}

void Amplifier::eqInstruction(Instruction ins) {
    int diff = 0;

    diff = getValueByMode(ins.param1, ins.mode1) -
           getValueByMode(ins.param2, ins.mode2);

    v[ins.param3] = (diff == 0);
    ip += opSizes.at(EQ);
}
