#include <iostream>
#include <fstream>

int main()
{
    std::ifstream fin("data.in");
    std::string str;
    int res1 = 0, res2 = 0;

    while (std::getline(fin, str)) {
        int i = 0, min1, min2, w = 0, l = 0, h = 0;

        for ( ; str[i] >= '0' && str[i] <= '9'; ++ i)
            l = l * 10 + str[i] - '0';

        for (++ i; str[i] >= '0' && str[i] <= '9'; ++ i)
            w = w * 10 + str[i] - '0';

        for (++ i; str[i] >= '0' && str[i] <= '9'; ++ i)
            h = h * 10 + str[i] - '0';

        if (w <= l && w <= h) {
            min1 = w;
            min2 = std::min(l, h);
        }
        else if (l <= w && l <= h) {
            min1 = l;
            min2 = std::min(w, h);
        }
        else {
            min1 = h;
            min2 = std::min (w, l);
        }

        res2 += 2 * (min1 + min2) + w * l * h;
        res1 += 2 * l * w + 2 * l * h + 2 * h * w + min1 * min2;
    }

    std::cout << "The answer for part1 is: " << res1 << "\n";
    std::cout << "The answer for part2 is: " << res2 << "\n";

    fin.close();

    return 0;
}
