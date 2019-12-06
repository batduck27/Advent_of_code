#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <map>

typedef struct Instruction {
    std::string name;
    char reg;
    int offset;

    Instruction() : name(), reg(), offset() {

    }

    Instruction(std::string _name, char _reg, int _offset)
        : name(_name), reg(_reg), offset(_offset) {
        
    }

    Instruction(std::string _name, char _reg)
        : name(_name), reg(_reg), offset() {
        
    }

    Instruction(std::string _name, int _offset)
        : name(_name), reg(), offset(_offset) {
        
    }
} Instruction;

long long run(const std::vector<Instruction>& I, bool part2 = false) {
    std::map<char, long long> M;
    int ip = 0;

    if (part2) {
        M['a'] = 1;
    }

    while (ip >= 0 && ip < (int)I.size()) {
        if (I[ip].name == "jmp") {
            ip += I[ip].offset;
            continue;
        } else if (I[ip].name == "jio") {
            if (M[I[ip].reg] == 1) {
                ip += I[ip].offset;
                continue;
            }
        } else if (I[ip].name == "jie") {
            if (!(M[I[ip].reg] & 1)) {
                ip += I[ip].offset;
                continue;
            }
        } else {
            if (I[ip].name == "inc") {
                ++ M[I[ip].reg];
            } else if (I[ip].name == "tpl") {
                M[I[ip].reg] *= 3;
            } else if (I[ip].name == "hlf") {
                M[I[ip].reg] /= 2;
            } else {
                std::cerr << "Instruction not found... " << I[ip].name << "\n";
            }
        }

        ++ ip;
    }

    return M['b'];
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    std::vector<Instruction> I;

    const std::regex jumpIfReg("(\\w+) (\\w), ((\\+|\\-)\\d+)");
    const std::regex jumpReg("(jmp) ((\\+|\\-)\\d+)");
    const std::regex unaryOpReg("(inc|hlf|tpl) (\\w)");

    while (std::getline(fin, str)) {
        std::smatch words;

        if (std::regex_match(str, words, jumpIfReg)) {
            I.push_back(Instruction(words[1], words[2].str()[0], std::stoi(words[3])));
        } else if (std::regex_match(str, words, jumpReg)) {
            I.push_back(Instruction(words[1], std::stoi(words[2])));
        } else if (std::regex_match(str, words, unaryOpReg)) {
            I.push_back(Instruction(words[1], words[2].str()[0]));
        } else {
            // do nothing
        }
    }

    std::cout << "The answer for part1 is: " << run(I) << "\n";
    std::cout << "The answer for part2 is: " << run(I, true) << "\n";

    fin.close();

    return 0;
}
