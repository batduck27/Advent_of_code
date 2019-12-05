#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

static const int DIM = 16;
static const int COUNTER = 1000000000;

void spin(std::string& V, int steps)
{
    std::rotate(V.rbegin(), V.rbegin() + (steps % DIM), V.rend());
}

void exchange(std::string& V, size_t A, size_t B)
{
    std::swap(V[A], V[B]);
}

void partner(std::string& V, char A, char B)
{
    size_t i, j;

    for (i = 0; V[i] != A && i < V.size(); ++ i);
    for (j = 0; V[j] != B && i < V.size(); ++ j);

    exchange(V, i, j);
}

std::vector<std::string> instructionSplit(std::string buffer)
{
    std::vector<std::string> I;
    size_t last = 0;
    size_t next = buffer.find_first_of(",");

    while (next != std::string::npos) {
        I.push_back(buffer.substr(last, next - last));

        last = next + 1;
        next = buffer.find_first_of(",", last);
    }

    I.push_back(buffer.substr(last));

    return I;
}

void dance(std::string& V, const std::vector<std::string>& ins)
{
    for (const auto& x : ins) {
        switch (x[0]) {
            case 's':
                spin(V, std::stoi(x.substr(1)));
                break;
            case 'x':
            {
                size_t ind = x.find_first_of("/");
                int A = std::stoi(x.substr(1, ind - 1));
                int B = std::stoi(x.substr(ind + 1));

                exchange(V, (size_t)A, (size_t)B);
                break;
            }
            case 'p':
                partner(V, x[1], x[3]);
                break;
            default:
                std::cout << "[Error]: Invalid instruction!\n";
        }
    }
}

int main()
{
    std::ifstream fin("data.in");
    std::string buffer;
    std::string V;
    std::vector<std::string> ins;
    std::map<std::string, int> M;

    std::getline(fin, buffer);
    fin.close();

    for (int i = 0; i < DIM; ++ i) {
        V.push_back((char)('a' + i));
    }

    ins = instructionSplit(buffer);
    dance(V, ins);
    std::cout << "The answer for part1 is: " << V << "\n";

    while (M.find(V) == M.end()) {
        M[V] = (int)M.size();
        dance(V, ins);
    }

    for (const auto& x : M) {
        if (x.second == (int)(COUNTER % M.size())) {
            std::cout << "The answer for part2 is: " << x.first << "\n";
            break;
        }
    }

    return 0;
}