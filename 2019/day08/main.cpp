#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

static const int INF = (1 << 30);
static const int HEIGHT = 6;
static const int WIDTH = 25;
static const int IMAGESIZE = HEIGHT * WIDTH;

enum PIXEL {
    BLACK = 0,
    WHITE,
    TRANSPARENT,
    PIXEL_NO,
};

int main() {
    std::ifstream fin("data.in");
    std::string str;
    int image[IMAGESIZE];
    int pixels[PIXEL_NO];
    int ind = 0;
    int minBlacksCnt = INF, sol1 = 0;

    std::getline(fin, str);
    std::fill(image, image + IMAGESIZE, 2);
    std::fill(pixels, pixels + PIXEL_NO, 0);

    for (int i = 0; i < (int)str.size(); ++ i) {
        int px = str[i] - '0';

        if (px != TRANSPARENT && image[ind] == TRANSPARENT) {
            image[ind] = px;
        }

        ++ ind;
        pixels[px] ++;

        if (ind == IMAGESIZE) {
            if (pixels[BLACK] < minBlacksCnt) {
                minBlacksCnt = pixels[BLACK];
                sol1 = pixels[WHITE] * pixels[TRANSPARENT];
            }

            std::fill(pixels, pixels + PIXEL_NO, 0);
            ind = 0;
        }
    }

    std::cout << "The answer for part 1 is: " << sol1 << "\n";
    std::cout << "The answer for part 2 is:";

    for (int i = 0; i < IMAGESIZE; ++ i) {
        if (i % WIDTH == 0) {
            std::cout << "\n";   
        }

        std::cout << ((image[i] == 0) ? ' ' : '#');
    }

    std::cout << "\n";

    return 0;
}
