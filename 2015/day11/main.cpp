#include <iostream>
#include <fstream>
#include <algorithm>

static const int I_CH = 'i' - 'a';
static const int L_CH = 'l' - 'a';
static const int O_CH = 'o' - 'a';
static const int MOD  = 'z' - 'a';
static const int COND_CNT = 2;

static bool checkCondition1(const std::string& str) {
    int lastCh = str[0];
    int cnt = 0;

    for (int i = 1; i < (int)str.size() && cnt < COND_CNT; ++ i) {
        if (str[i] - 1 == lastCh) {
            cnt ++;
        } else {
            cnt = 0;
        }

        lastCh = str[i];
    }

    return (cnt == COND_CNT);
}

static bool checkCondition2(const std::string& str) {
    return !std::any_of(str.begin(), str.end(), [](char ch) { return ch == I_CH ||
                                                                     ch == L_CH ||
                                                                     ch == O_CH; });
}

static bool checkCondition3(const std::string& str) {
    int cnt = 0;

    for (int i = 1; i < (int)str.size() && cnt < COND_CNT; ++ i) {
        if (str[i] == str[i - 1]) {
            ++ cnt;
            ++ i;
        }
    }

    return (cnt == COND_CNT);
}

static bool passIsValid(const std::string& str) {
    return checkCondition1(str) &&
           checkCondition2(str) &&
           checkCondition3(str);
}

static void incrementPassword(std::string& str) {
    bool carry = false;
    int ind = str.size() - 1;

    ++ str[ind];

    if (str[ind] > MOD) {
        carry = true;
        str[ind] = 0;
    }

    while (carry) {
        carry = false;
        -- ind;
        ++ str[ind];

        if (str[ind] > MOD) {
            carry = true;
            str[ind] = 0;
        }
    }
}

static std::string generateNextPassword(std::string& str) {
    for (int i = 0; i < (int)str.size(); ++ i) {
        str[i] -= 'a';
    }

    do {
        incrementPassword(str);
    } while (!passIsValid(str));

    for (int i = 0; i < (int)str.size(); ++ i) {
        str[i] += 'a';
    }

    return str;
}

int main() {
    std::ifstream fin("data.in");
    std::string str;

    fin >> str;

    std::cout << "The answer for part1 is: " << generateNextPassword(str) << "\n";
    std::cout << "The answer for part2 is: " << generateNextPassword(str) << "\n";

    fin.close();

    return 0;
}
