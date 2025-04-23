#pragma once

#include <vector>

enum DIRECTION { UP, LEFT, DOWN, RIGHT };

struct Point {
    int x, y;

    explicit Point(const int x = 0, const int y = 0) : x(x), y(y) {}
};

class Airplane {
public:
    Point              head;
    std::vector<Point> body;
    bool               alive = true;

    Airplane(int x, int y, DIRECTION direction);
    [[nodiscard]] bool isHead(const Point& p) const;
    [[nodiscard]] bool isBody(const Point& p) const;
};
