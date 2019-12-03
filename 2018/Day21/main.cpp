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
    {"bori", [](int reg[], const Args& a)->void { reg[a.C] = reg[a.A] | a.B; }},
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

std::vector<int> salutare;

int part1(const std::vector<Instruction>& I, int ipReg) {
    int reg[REG_NR] = {0};
    int ip = 0;
    int last = -1;

    while (ip >= 0 && ip < I.size()) {
        reg[ipReg] = ip;
        I[ip].f(reg, I[ip].a);
        ip = reg[ipReg] + 1;

        if (ip == 29) {
            return reg[I[28].a.A];
        }
    }

    return reg[0];
}

int part2(const std::vector<Instruction>& I, int ipReg) {
    int last = -1;
    std::set<int> seen;
    bool sw = true;
    int x = 0, y = 0, z = 0;

    while (true) {
        if (sw) {
            y = z | I[6].a.B;
            z = I[7].a.A;
        }

        sw = true;
        x = y & I[8].a.B;
        z = (((x + z) & I[10].a.B) * I[11].a.B) & I[12].a.B;

        if (y < I[13].a.A) {
            if (seen.find(z) != seen.end()) {
                break;
            } else {
                seen.insert(z);
                last = z;
            }
        } else {
            x = y = y / I[19].a.B;
            sw = false;
        }
    }

    return last;
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    std::vector<Instruction> I;
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

    std::cout << "The answer for part1 is: " << part1(I, ipReg) << "\n";
    std::cout << "The answer for part2 is: " << part2(I, ipReg) << "\n";

    fin.close();

    return 0;
}