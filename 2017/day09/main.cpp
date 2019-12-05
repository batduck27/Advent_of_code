#include <iostream>
#include <fstream>

int main()
{
    std::ifstream fin("data.in");
    std::string buffer;
    bool garbage = false;
    int totalScore = 0;
    int garbCounter = 0;
    int groupCounter = 0;

    std::getline(fin, buffer);
    fin.close();

    for (size_t ind = 0; ind < buffer.size(); ++ ind) {
        if (buffer[ind] == '!') {
            ++ ind;
            continue;
        }

        if (garbage == false && buffer[ind] == '<') {
            garbage = true;
            continue;
        }

        if (garbage == true && buffer[ind] == '>') {
            garbage = false;
            continue;
        }

        if (garbage == true) {
            ++ garbCounter;
            continue;
        }

        if (garbage == false) {
            if (buffer[ind] == '{') {
                ++ groupCounter;
            } else if (buffer[ind] == '}') {
                totalScore += groupCounter;
                -- groupCounter;
            }
        }
    }

    std::cout << "The answer for part1 is: " << totalScore << "\n";
    std::cout << "The answer for part2 is: " << garbCounter << "\n";

    return 0;
}