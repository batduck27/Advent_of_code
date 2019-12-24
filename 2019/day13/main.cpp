#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>

static const long long INPUT_PART2 = 2LL;

enum TILES {
    EMPTY = 0,
    WALL,
    BLOCK,
    PADDLE,
    BALL,
};

enum JOYSTICK_TILT {
    LEFT = -1,
    NEUTRAL = 0,
    RIGHT = 1,
};

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

int lel;

class ArcadeMachine {
private:
    std::queue<long long> input;
    std::deque<long long> output;
    std::unordered_map<long long, long long> v;
    std::unordered_map<long long, long long> copyV;
    long long ip; // instruction pointer
    long long relativeBase;
    long long lastScore;
    long long ballX;
    long long paddleX;
    long long blocksCnt;

    Instruction getNextInstruction();
    long long getValueByMode(long long param, enum PARAM_MODE mode);
    void setValueByMode(long long param, enum PARAM_MODE mode, long long val);
    void provideInput();

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
    ArcadeMachine() 
        : input(), output(), v(), copyV(), ip(), relativeBase(), ballX(), paddleX(), blocksCnt()
    {

    }

    ArcadeMachine(std::unordered_map<long long, long long> v)
        : input(), output(), v(v), copyV(v), ip(), relativeBase(), ballX(), paddleX(), blocksCnt()
    {

    }

    bool hasHalted();
    long long run(bool part2 = false);
    void reset();
    void hack();
};

int main() {
    std::ifstream fin("data.in");
    std::unordered_map<long long, long long> v;
    long long x, i = 0;

    while (fin >> x) {
        v[i ++] = x;
        fin.ignore();
    }

    fin.close();

    ArcadeMachine arcade{v};

    std::cout << "The answer for part1 is: " << arcade.run() << "\n";
    arcade.reset();
    arcade.hack();
    std::cout << "The answer for part2 is: " << arcade.run(true) << "\n";

    return 0;
}

void ArcadeMachine::hack() {
    v[0] = 2;
}

long long ArcadeMachine::getValueByMode(long long param, enum PARAM_MODE mode) {
    if (mode == POSITION) {
        return v[param];
    }

    if (mode == RELATIVE) {
        return v[param + relativeBase];
    }

    return param;
}

void ArcadeMachine::setValueByMode(long long param, enum PARAM_MODE mode, long long val) {
    if (mode == POSITION) {
        v[param] = val;
    } else if (mode == RELATIVE) {
        v[param + relativeBase] = val;
    } else {
        std::cout << "SetValueByMode called with IMMEDIATE mode. HALTING\n";
        ip = -1;
    }
}

void ArcadeMachine::provideInput() {
    long long tilt = NEUTRAL;

    if (ballX > paddleX) {
        tilt = RIGHT;
    } else if (ballX < paddleX) {
        tilt = LEFT;
    }

    input.push(tilt);
}

void ArcadeMachine::reset() {
    ip           = 0;
    relativeBase = 0;
    v            = copyV;
    lastScore    = 0;
    paddleX      = 0;
    ballX        = 0;
    blocksCnt    = 0; 

    output.clear();

    while (!input.empty()) {
        input.pop();
    }
} 

bool ArcadeMachine::hasHalted() {
    return (ip < 0);
}

long long ArcadeMachine::run(bool part2) {
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
            if (part2) {
                provideInput();
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

    if (!part2) {
        return blocksCnt;
    }

    return lastScore;
}

Instruction ArcadeMachine::getNextInstruction() {
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

void ArcadeMachine::addInstruction(Instruction ins) {
    long long tmp = getValueByMode(ins.param1, ins.mode1) +
                    getValueByMode(ins.param2, ins.mode2);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(ADD);
}

void ArcadeMachine::mulInstruction(Instruction ins) {
    long long tmp = getValueByMode(ins.param1, ins.mode1) *
                    getValueByMode(ins.param2, ins.mode2);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(MUL);
}

void ArcadeMachine::inInstruction(Instruction ins) {
    if (input.empty()) {
        std::cout << "No input in queue. HALTING\n";
        ip = -1;
    }

    setValueByMode(ins.param1, ins.mode1, input.front());
    input.pop();
    ip += opSizes.at(IN);
}

void ArcadeMachine::outInstruction(Instruction ins) {
    long long diagCode = getValueByMode(ins.param1, ins.mode1);
    output.push_back(diagCode);

    if (output.size() == 3) {
        long long x  = output[0];
        long long y  = output[1];
        long long id = output[2];

        if (x == -1LL && y == 0LL) {
            lastScore = id;
        } else if (id == PADDLE) {
            paddleX = x;
        } else if (id == BALL) {
            ballX = x;
        } else if (id == BLOCK) {
            blocksCnt ++;
        }

        output.pop_front();
        output.pop_front();
        output.pop_front();
    }
    
    ip += opSizes.at(OUT);
}

void ArcadeMachine::jmptInstruction(Instruction ins) {
    if (getValueByMode(ins.param1, ins.mode1) != 0) {
        ip = getValueByMode(ins.param2, ins.mode2);
    } else {
        ip += opSizes.at(JMPT);
    }
}

void ArcadeMachine::jmpfInstruction(Instruction ins) {
    if (getValueByMode(ins.param1, ins.mode1) == 0) {
        ip = getValueByMode(ins.param2, ins.mode2);
    } else {
        ip += opSizes.at(JMPF);
    }
}

void ArcadeMachine::ltInstruction(Instruction ins) {
    long long tmp = ((getValueByMode(ins.param1, ins.mode1) <
                      getValueByMode(ins.param2, ins.mode2)) ? 1LL : 0LL);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(LT);
}

void ArcadeMachine::eqInstruction(Instruction ins) {
    long long tmp = ((getValueByMode(ins.param1, ins.mode1) ==
                      getValueByMode(ins.param2, ins.mode2)) ? 1LL : 0LL);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(EQ);
}

void ArcadeMachine::roffInstruction(Instruction ins) {
    relativeBase += getValueByMode(ins.param1, ins.mode1);
    ip += opSizes.at(ROFF);
}
