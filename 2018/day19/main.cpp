#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <functional>
#include <sstream>

#include <stdio.h>

#define REG_NR     (int)(6)

typedef struct Args {
    int A, B, C;

    Args() : A(), B(), C() {

    }

    Args(int _A, int _B, int _C) : A(_A), B(_B), C(_C) {
        
    }
} Args;

typedef struct Instruction {
    std::function<void(int*, const Args&)> f;
    Args a;

    Instruction() : f(), a() {

    }

    Instruction(std::function<void(int*, const Args&)> _f, int _A, int _B, int _C)
        : f(_f), a(_A, _B, _C) {

    }
} Instruction;

const std::map<std::string, std::function<void(int*, const Args&)>> functors {
    // addr
    {"addr", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] + reg[a.B]; }},
    // addi
    {"addi", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] + a.B; }},
    // mulr
    {"mulr", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] * reg[a.B]; }},
    // muli
    {"muli", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] * a.B; }},
    // banr
    {"banr", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] & reg[a.B]; }},
    // bani
    {"bani", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] & a.B; }},
    // borr
    {"borr", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] | reg[a.B]; }},
    // borri
    {"borri", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] | a.B; }},
    // setr
    {"setr", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A]; }},
    // seti
    {"seti", [](int reg[], const Args& a)->void { reg[a.C] = a.A; }},
    // gtir
    {"gtir", [](int reg[], const Args& a)->void { reg[a.C] = a.A > reg[a.B]; }},
    // gtri
    {"gtri", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] > a.B; }},
    // gtrr
    {"gtrr", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] > reg[a.B]; }},
    // eqir
    {"eqir", [](int reg[], const Args& a)->void { reg[a.C] = a.A == reg[a.B]; }},
    // eqri
    {"eqri", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] == a.B; }},
    // eqrr
    {"eqrr", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] == reg[a.B]; }}
};

int part1(const std::vector<Instruction>& I, int ipReg, bool part2 = false) {
    int reg[REG_NR] = {0};
    int ip = 0;

    while (ip >= 0 && ip < I.size()) {
        reg[ipReg] = ip;
        I[ip].f(reg, I[ip].a);
        ip = reg[ipReg] + 1;
    }

    return reg[0];
}

int part2(const std::vector<Instruction>& I, int ipReg) {
    int nrPart1 = (I[17].a.B * I[17].a.B * 19 * I[20].a.B + I[21].a.B * 22 + I[23].a.B);
    int toAddPart2 = ((27 * 28 + 29) * 30 * I[31].a.B * 32);
    int nr = nrPart1 + toAddPart2;
    int res = 0;

    for (int i = 1; i <= nr; ++ i) {
        res += (nr % i == 0) ? i : 0; 
    }

    return res;
}

// this is the pseudocode resulted after analysis of my input - registers may differ on other inputs
// this won't work for part2 :D
int pseudocode(const std::vector<Instruction>& I, int ipReg, bool part2 = false) {
    int reg[REG_NR] = {0};
    int ip = 0;
    int nrPart1 = (I[17].a.B * I[17].a.B * 19 * I[20].a.B + I[21].a.B * 22 + I[23].a.B);
    int toAddPart2 = ((27 * 28 + 29) * 30 * I[31].a.B * 32);

    reg[2] = nrPart1 + ((part2) ? toAddPart2 : 0);
    reg[1] = 1;
    reg[3] = 1;

    while (true) {
        if (reg[1] * reg[3] == reg[2]) {
            reg[0] += reg[1];
        }

        ++ reg[3];

        if (reg[3] > reg[2]) {
            ++ reg[1];
            
            if (reg[1] <= reg[2]) {
                reg[3] = 1;
            } else {
                break;
            }
        }
    }

    return reg[0];
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    std::vector<Instruction> I;
    int reg[REG_NR] = {0};
    int ip = 0, ipReg;

    std::getline(fin, str);
    sscanf(str.c_str(), "#ip %d", &ipReg);

    while (std::getline(fin, str)) {
        std::istringstream iss(str);
        std::string funcName;
        int A, B, C;

        iss >> funcName >> A >> B >> C;

        auto i = functors.find(funcName);

        if (i != functors.end()) {
            I.push_back(Instruction(i->second, A, B, C));
        } else {
            std::cerr << "Invalid instruction: " << str << "\n";
        }
    }

    std::cout << "The answer for part1 is: " << part1(I, ipReg)
              << "  // Also, the pseudocode returned: " << pseudocode(I, ipReg) << "\n";
    std::cout << "The answer for part2 is: " << part2(I, ipReg) << "\n";

    fin.close();

    return 0;
}