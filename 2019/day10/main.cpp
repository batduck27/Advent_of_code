#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <utility>
#include <tuple>
#include <algorithm>

#include <math.h>

#define METEOR '#'

static const int VAPORIZED_CNT = 200;

typedef std::tuple<int, int, int> III;

typedef struct Point {
    int x, y;

    Point() : x(), y() {

    }

    Point(int x, int y) : x(x), y(y) {

    }
} Point;

Point operator +(Point a, Point b) {
    return Point{a.x + b.x, a.y + b.y};
}

Point operator -(Point a, Point b) {
    return Point{a.x - b.x, a.y - b.y};
}

bool operator ==(Point a, Point b) {
    return ((a.x == b.x) && (a.y == b.y));
}

bool operator <(Point a, Point b) {
    if (a.x == b.x) {
        return a.y < b.y;
    }

    return a.x < b.x;
}

double getAngle(Point a, Point b) {
    Point normalizedP = b - a;
    double angle = atan2(normalizedP.y, normalizedP.x);

    if (angle < (-1) * M_PI / 2) {
        angle += 2 * M_PI;
    }

    return angle;
}

III getLineFromPoints(Point p1, Point p2) {
    int a = p2.y - p1.y;
    int b = p1.x - p2.x;
    int c = a * p1.x + b * p1.y;

    return III{a, b, c};
}

std::vector<std::pair<double, Point>> getVisiblePoints(const std::set<Point>& points, Point orig) {
    std::vector<std::pair<double, Point>> visiblePoints;

    for (const auto& p : points) {
        int minX, minY, maxX, maxY;
        bool noIntersections = true;

        if (p == orig) {
            continue;
        }

        minX = std::min(p.x, orig.x);
        maxX = std::max(p.x, orig.x);
        minY = std::min(p.y, orig.y);
        maxY = std::max(p.y, orig.y);

        for (const auto& intPoint : points) {
            if (intPoint == orig || intPoint == p) {
                continue;
            }

            if (intPoint.x < minX || intPoint.x > maxX ||
                intPoint.y < minY || intPoint.y > maxY)
            {
                continue;
            }

            auto [a, b, c] = getLineFromPoints(orig, p);

            if (a * intPoint.x + b * intPoint.y == c) {
                noIntersections = false;
                break;
            }
        }

        if (noIntersections) {
            visiblePoints.push_back(std::make_pair(getAngle(orig, p), p));
        }
    }

    return visiblePoints;
}

int main() {
    std::ifstream fin("data.in");
    std::string str;
    std::set<Point> points;
    std::vector<std::pair<double, Point>> bestVisiblePoints{};

    for (int i = 0; std::getline(fin, str); ++ i) {
        for (int j = 0; j < (int)str.size(); ++ j) {
            if (str[j] == METEOR) {
                points.insert(Point{j, i});
            }
        }
    }

    fin.close();

    for (const auto& p : points) {
        std::vector<std::pair<double, Point>> visiblePoints = getVisiblePoints(points, p);

        if (visiblePoints.size() > bestVisiblePoints.size()) {
            bestVisiblePoints = visiblePoints;
        }
    }

    std::sort(bestVisiblePoints.begin(), bestVisiblePoints.end());

    std::cout << "The answer for part1 is: " << bestVisiblePoints.size() << "\n";

    if (bestVisiblePoints.size() < VAPORIZED_CNT) {
        std::cout << "There are not enough points!\n";
    } else {
        auto [x, y] = bestVisiblePoints[VAPORIZED_CNT - 1].second;

        std::cout << "The answer for part2 is: " << x * 100 + y << "\n";
    }

    return 0;
}
