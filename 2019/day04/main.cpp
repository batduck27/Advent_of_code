#include <iostream>
#include <fstream>

int main() {
    std::ifstream fin("data.in");
    int sol1 = 0, sol2 = 0;
    int a, b;

    fin >> a;
    fin.ignore();
    fin >> b;
    fin.close();

    for (int i = a; i <= b; ++ i) {
        bool cond1 = false; // matching adjacent digits condition
        bool cond2 = true;  // non-decreasing order condition
        bool cond3 = false; // matching adjacent digits not in larger group condition
        int group  = 1;     // the size of the current same digit group 
        int x = i;
        int lastDigit = x % 10;

        do {
            int currDigit;

            x /= 10;
            currDigit = x % 10;

            if (currDigit == lastDigit) {
                // the first condition was met (matching adjacent digits)
                cond1 = true;
                group ++;
            } else {
                if (group == 2) {
                    // the third condition was met (there is are 2 matching adjacent
                    // digits which are not part of a larger group of matching digits)
                    cond3 = true;
                }

                group = 1;
            }

            if (lastDigit < currDigit) {
                // the second condition was not met :(
                cond2 = false;
            }

            lastDigit = currDigit;
        } while (x > 0);

        sol1 += (cond1 && cond2);
        sol2 += (cond1 && cond2 && cond3);
    }

    std::cout << "The answer for part 1 is: " << sol1 << "\n";
    std::cout << "The answer for part 2 is: " << sol2 << "\n";

    return 0;
}
