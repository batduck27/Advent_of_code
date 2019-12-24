#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <algorithm>

static const int INF = (1 << 30);
static const int vert[] = {0, 0, -1, 1};
static const int oriz[] = {-1, 1, 0, 0};

enum Color {
    BLACK,
    WHITE,
};

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN,
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

class PaintingRobot {
private:
    std::queue<long long> input;
    std::unordered_map<long long, long long> v;
    std::unordered_map<long long, long long> copyV;
    long long ip; // instruction pointer
    long long relativeBase;

    int dir, x, y;
    long long outputCnt;

    Instruction getNextInstruction();
    long long getValueByMode(long long param, enum PARAM_MODE mode);
    void setValueByMode(long long param, enum PARAM_MODE mode, long long val);
    void provideInput();
    void getNewDirection(enum Direction turn);

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
    PaintingRobot()
        : input(), v(), copyV(), ip(), relativeBase(), dir(UP), x(), y(), outputCnt()
    {

    }

    PaintingRobot(std::unordered_map<long long, long long> v)
        : input(), v(v), copyV(v), ip(), relativeBase(), dir(UP), x(), y(), outputCnt()
    {

    }

    bool hasHalted();
    long long run();
    void reset();
};

std::map<std::pair<int, int>, int> painted;

void printRegistrationIdentifier() {
    int minY = INF, maxY = -INF, minX = INF, maxX = -INF;

    for (const auto& x : painted) {
        minX = std::min(x.first.first, minX);
        maxX = std::max(x.first.first, maxX);
        minY = std::min(x.first.second, minY);
        maxY = std::max(x.first.second, maxY);
    }

    for (int i = minY; i <= maxY; ++ i) {
        for (int j = minX; j <= maxX; ++ j) {
            if (painted[std::make_pair(j, i)] == WHITE) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
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

    PaintingRobot robot{v};

    robot.reset();
    robot.run();
    std::cout << "The answer for part1 is: " << painted.size() << "\n";    

    painted.clear();
    painted[std::make_pair(0, 0)] = WHITE;
    robot.reset();
    robot.run();
    std::cout << "The answer for part2 is:\n";
    printRegistrationIdentifier();

    return 0;
}

long long PaintingRobot::getValueByMode(long long param, enum PARAM_MODE mode) {
    if (mode == POSITION) {
        return v[param];
    }

    if (mode == RELATIVE) {
        return v[param + relativeBase];
    }

    return param;
}

void PaintingRobot::setValueByMode(long long param, enum PARAM_MODE mode, long long val) {
    if (mode == POSITION) {
        v[param] = val;
    } else if (mode == RELATIVE) {
        v[param + relativeBase] = val;
    } else {
        std::cout << "SetValueByMode called with IMMEDIATE mode. HALTING\n";
        ip = -1;
    }
}

void PaintingRobot::provideInput() {    
    if (painted.find(std::make_pair(x, y)) == painted.end()) {
        input.push(BLACK);
    } else {
        input.push(painted[std::make_pair(x, y)]);
    }
}

void PaintingRobot::getNewDirection(enum Direction turn) {
    switch (dir) {
    case UP:
        dir = turn;
        break;
    case RIGHT:
        dir = UP + turn;
        break;
    case DOWN:
        dir = RIGHT - turn;
        break;
    case LEFT:
        dir = DOWN - turn;
        break;
    }
}

void PaintingRobot::reset() {
    ip           = 0;
    relativeBase = 0;
    v            = copyV;
    dir          = UP;
    x            = 0;
    y            = 0;
    outputCnt    = 0;

    while (!input.empty()) {
        input.pop();
    }
} 

bool PaintingRobot::hasHalted() {
    return (ip < 0);
}

long long PaintingRobot::run() {
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
            provideInput();
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

    return 0;
}

Instruction PaintingRobot::getNextInstruction() {
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

void PaintingRobot::addInstruction(Instruction ins) {
    long long tmp = getValueByMode(ins.param1, ins.mode1) +
                    getValueByMode(ins.param2, ins.mode2);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(ADD);
}

void PaintingRobot::mulInstruction(Instruction ins) {
    long long tmp = getValueByMode(ins.param1, ins.mode1) *
                    getValueByMode(ins.param2, ins.mode2);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(MUL);
}

void PaintingRobot::inInstruction(Instruction ins) {
    if (input.empty()) {
        std::cout << "No input in queue. HALTING\n";
        ip = -1;
        return;
    }

    setValueByMode(ins.param1, ins.mode1, input.front());
    input.pop();
    ip += opSizes.at(IN);
}

void PaintingRobot::outInstruction(Instruction ins) {
    long long diagCode;

    diagCode = getValueByMode(ins.param1, ins.mode1);
    ip += opSizes.at(OUT);

    if (!(outputCnt & 1)) {
        // first value - color of the cell
        painted[std::make_pair(x, y)] = (int)diagCode;
    } else {
        getNewDirection((enum Direction)diagCode);

        x += oriz[dir];
        y += vert[dir];
    }

    outputCnt ++;
}

void PaintingRobot::jmptInstruction(Instruction ins) {
    if (getValueByMode(ins.param1, ins.mode1) != 0) {
        ip = getValueByMode(ins.param2, ins.mode2);
    } else {
        ip += opSizes.at(JMPT);
    }
}

void PaintingRobot::jmpfInstruction(Instruction ins) {
    if (getValueByMode(ins.param1, ins.mode1) == 0) {
        ip = getValueByMode(ins.param2, ins.mode2);
    } else {
        ip += opSizes.at(JMPF);
    }
}

void PaintingRobot::ltInstruction(Instruction ins) {
    long long tmp = ((getValueByMode(ins.param1, ins.mode1) <
                      getValueByMode(ins.param2, ins.mode2)) ? 1LL : 0LL);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(LT);
}

void PaintingRobot::eqInstruction(Instruction ins) {
    long long tmp = ((getValueByMode(ins.param1, ins.mode1) ==
                      getValueByMode(ins.param2, ins.mode2)) ? 1LL : 0LL);

    setValueByMode(ins.param3, ins.mode3, tmp);
    ip += opSizes.at(EQ);
}

void PaintingRobot::roffInstruction(Instruction ins) {
    relativeBase += getValueByMode(ins.param1, ins.mode1);
    ip += opSizes.at(ROFF);
}
