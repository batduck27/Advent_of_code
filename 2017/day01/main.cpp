#include <fstream>
#include <iostream>

int main()
{
    std::string input;
    std::ifstream fin("data.in");
    int captcha1 = 0;
    int captcha2 = 0;

    fin >> input;

    for (int i = 0; i < input.size(); ++ i) {
        int ind = (i + 1) % input.size();
        
        if (input[i] == input[ind]) {
            captcha1 += input[i] - '0';
        }
    
        ind = (i + (input.size() / 2)) % input.size();

        if (input[i] == input[ind]) {
            captcha2 += input[i] - '0';
        }
    }

    std::cout << "The answer for part1 is: " << captcha1 << "\n";
    std::cout << "The answer for part2 is: " << captcha2 << "\n";
    
    fin.close();

    return 0;
}