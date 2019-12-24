#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>

enum OPCODE {
    ADD  = 1,
    MUL  = 2,
    IN   = 3,
    OUT  = 4,
    JMPT = 5,
    JMPF = 6,
    LT   = 7,
    EQ   = 8,
    ROFF = 9,
    HALT = 99,
};

enum PARAM_MODE {
    POSITION = 0,
    IMMEDIATE = 1,
    RELATIVE = 2,
};

static const std::unordered_map<enum OPCODE, int> opSizes{
    { ADD,  4 },
    { MUL,  4 },
    { IN,   2 },
    { OUT,  2 },
    { JMPT, 3 },
    { JMPF, 3 },
    { LT,   4 },
    { EQ,   4 },
    { ROFF, 2 },
    { HALT, 1 },
};

typedef struct Instruction {
    long long opcode;
    long long param1, param2, param3;
    enum PARAM_MODE mode1, mode2, mode3;
} Instruction;

class IntCodeComputer {
private:
    std::queue<long long> input;
    std::unordered_map<long long, long long> v;
    std::unordered_map<long long, long long> copyV;
    long long ip; // instruction pointer
    long long diagCode;
    long long relativeBase;

    Instruction getNextInstruction();
    long long getValueByMode(long long param, enum PARAM_MODE mode);
    void setValueByMode(long long param, enum PARAM_MODE mode, long long val);

    void addInstruction(Instruction ins);
    void mulInstruction(Instruction ins);
    void inInstruction(Instruction ins);
    void outInstruction(Instruction ins);
    void jmptInstruction(Instruction ins);
    void jmpfInstruction(Instruction ins);
    void ltInstruction(Instruction ins);
    void eqInstruction(Instruction ins);
    void roffInstruction(Instruction ins);

public:
    IntCodeComputer() : input(), v(), copyV(), ip(), diagCode(), relativeBase() {
    }

    IntCodeComputer(std::unordered_map<long long, long long> v)
        : input(), v(v), copyV(v), ip(), diagCode(), relativeBase() {
    }

    bool hasHalted();
    long long run();
    void reset();
    void provideInput(long long in);
};

bool pointInBeam(IntCodeComputer& computer, int r, int c) {
    computer.reset();
    computer.provideInput(c);
    computer.provideInput(r);

    return computer.run();
}

int solvePart1(IntCodeComputer& computer, int size) {
    int points = 0;

    for (int r = 0; r < size; ++ r) {
        for (int c = r; c < size; ++ c) {
            points += pointInBeam(computer, r, c);
        }
    }

    return points;
}

int solvePart2(IntCodeComputer& computer, int size) {
    const int offset = size - 1;
    int r = size;
    int c = 0;

    while (true) {
        while (!pointInBeam(computer, r, c)) {
            ++ c;
        }

        if (pointInBeam(computer, r - offset, c) &&
            pointInBeam(computer, r - offset, c + offset))
        {
            break;   
        }

        ++ r;
    }

    return c * 10000 + r - offset;
}

int main() {
    std::ifstream fin("data.in");
    std::unordered_map<long long, long long> v;
    long long x, i = 0;

    while (fin >> x) {
        v[i ++] = x;
        fin.ignore();
    }

    fin.close();

    IntCodeComputer computer{v};

    std::cout << "The answer for part1 is: " << solvePart1(computer, 50) << "\n";
    std::cout << "The answer for part2 is: " << solvePart2(computer, 100) << "\n";

    return 0;
}

long long IntCodeComputer::getValueByMode(long long param, enum PARAM_MODE mode) {
    if (mode == POSITION) {
        return v[param];
    }

    if (mode == RELATIVE) {
        return v[param + relativeBase];
    }

    return param;
}

void IntCodeComputer::setValueByMode(long long param, enum PARAM_MODE mode, long long val) {
    if (mode == POSITION) {
        v[param] = val;
    } else if (mode == RELATIVE) {
        v[param + relativeBase] = val;
    } else {
        std::cout << "SetValueByMode called with IMMEDIATE mode. HALTING\n";
        ip = -1;
    }
}

void IntCodeComputer::provideInput(long long in) {
    input.push(in);
}

void IntCodeComputer::reset() {
    ip           = 0;
    diagCode     = 0;
    relativeBase = 0;
    v            = copyV;

    while (!input.empty()) {
        input.pop();
    }
} 

bool IntCodeComputer::hasHalted() {
    return (ip < 0);
}

long long IntCodeComputer::run() {
    diagCode = 0;

    while (!hasHalted()) {
        Instruction ins = getNextInstruction();

        switch (ins.opcode) {
        case ADD:
            addInstruction(ins);
            break;
        case MUL:
            mulInstruction(ins);
            break;
        case IN:
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
        case ROFF:
            roffInstruction(ins);
            break;
        case HALT:
            ip = -1;
            break;
        default:
            std::cout << "Invalid opcode " << ins.opcode << " at index " << ip << "\n";
            ip = -1;
            break;
        }
    }

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

    if (ins.opcode == IN || ins.opcode == OUT || ins.opcode == ROFF) {
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
    long long tmp = getValueByMode(ins.param1, ins.mode1) +
                    getValueByMode(ins.param2, ins.mode2);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(ADD);
}

void IntCodeComputer::mulInstruction(Instruction ins) {
    long long tmp = getValueByMode(ins.param1, ins.mode1) *
                    getValueByMode(ins.param2, ins.mode2);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(MUL);
}

void IntCodeComputer::inInstruction(Instruction ins) {
    if (input.empty()) {
        std::cout << "No input in queue. HALTING\n";
        ip = -1;
    }

    setValueByMode(ins.param1, ins.mode1, input.front());
    input.pop();
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
    long long tmp = ((getValueByMode(ins.param1, ins.mode1) <
                      getValueByMode(ins.param2, ins.mode2)) ? 1LL : 0LL);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(LT);
}

void IntCodeComputer::eqInstruction(Instruction ins) {
    long long tmp = ((getValueByMode(ins.param1, ins.mode1) ==
                      getValueByMode(ins.param2, ins.mode2)) ? 1LL : 0LL);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(EQ);
}

void IntCodeComputer::roffInstruction(Instruction ins) {
    relativeBase += getValueByMode(ins.param1, ins.mode1);
    ip += opSizes.at(ROFF);
}
