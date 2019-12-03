#include <iostream>
#include <fstream>
#include <map>

const std::string INIT = "initial state: ";
const std::string SEP = " => ";
const int WSIZE = 5;
const int PART1 = 20;
const long long PART2 = 50000000000;

std::map <std::string, char> M;
std::map <std::string, int> seen;
    
int potSum(std::string str, int firstPot) {
    int s = 0;

    for (size_t i = 0; i < str.size(); ++ i) {
        if (str[i] == '#') {
            s += (int)i + firstPot;
        }
    }

    return s;
}

int main() {
    std::ifstream fin("data.in");
    std::string state, tmpState;
    int firstPot = 0;

    std::getline(fin, state);
    fin.ignore(1, '\n');
    for (std::string str; std::getline(fin, str);) {
        int pos = str.find(SEP);
        M[str.substr(0, pos)] = str[pos + SEP.size()];
    }

    state = state.substr(INIT.size());

    int gen;

    for (gen = 1; ; ++ gen) {
        // add empty pots
        {
            int firstPlant = state.find('#');
            int lastPlant = state.find_last_of('#');

            state = std::string(WSIZE - firstPlant + 1, '.') +
                    state +
                    std::string(WSIZE - state.size() + lastPlant, '.'); 
            firstPot -= WSIZE - firstPlant + 1;
        }

        tmpState = state;

        for (size_t i = 0; i < state.size() - WSIZE; ++ i) {
            std::string window = state.substr(i, WSIZE);
            tmpState[i + WSIZE / 2] = (M.find(window) != M.end()) ? M[window] : '.';
        }       

        // delete those empty pots
        {
            int firstPlant = tmpState.find('#');
            int lastPlant = tmpState.find_last_of('#');

            state = tmpState.substr(firstPlant, lastPlant - firstPlant + 1);
            firstPot += firstPlant;
        }

        int ps = potSum(state, firstPot);

        if (gen == PART1) {
            std::cout << "The answer for part1 is: " << ps << "\n";
        }

        if (seen.find(state) != seen.end()) {
            std::cout << "The answer for part2 is: " << seen[state] + (PART2 - (gen - 1)) * (ps - seen[state]) << "\n";
            break;
        } else {
            seen[state] = ps;
        }
    }

    fin.close();

    return 0;
}