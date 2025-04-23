#include "Airplane.hpp"

Airplane::Airplane(const int x, const int y, const DIRECTION direction) : head(x, y) {
    if (direction == UP) {
        body = {
            Point(x + 1, y - 2), Point(x + 1, y - 1), Point(x + 1, y),
            Point(x + 1, y + 1), Point(x + 1, y + 2), Point(x + 2, y),
            Point(x + 3, y - 1), Point(x + 3, y + 1), Point(x + 3, y)
        };
    } else if (direction == LEFT) {
        body = {
            Point(x - 2, y + 1), Point(x - 1, y + 1), Point(x, y + 1),
            Point(x + 1, y + 1), Point(x + 2, y + 1), Point(x, y + 2),
            Point(x + 1, y + 3), Point(x - 1, y + 3), Point(x, y + 3)
        };
    } else if (direction == DOWN) {
        body = {
            Point(x - 1, y + 2), Point(x - 1, y + 1), Point(x - 1, y),
            Point(x - 1, y - 1), Point(x - 1, y - 2), Point(x - 2, y),
            Point(x - 3, y + 1), Point(x - 3, y - 1), Point(x - 3, y)
        };
    } else {
        body = {
            Point(x + 2, y - 1), Point(x + 1, y - 1), Point(x, y - 1),
            Point(x - 1, y - 1), Point(x - 2, y - 1), Point(x, y - 2),
            Point(x - 1, y - 3), Point(x + 1, y - 3), Point(x, y - 3)
        };
    }
}

bool Airplane::isHead(const Point& p) const { return p.x == head.x && p.y == head.y; }

bool Airplane::isBody(const Point& p) const {
    for (const auto& part : body) {
        if (p.x == part.x && p.y == part.y) {
            return true;
        }
    }
    return false;
}
