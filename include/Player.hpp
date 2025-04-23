#pragma once

#include <vector>
#include <string>
#include "Airplane.hpp"

class Player {
public:
    std::vector<Airplane>         airplanes;
    std::vector<std::vector<int>> grid;

    int planesDown = 0;

    Player();
    void placeAirplane(int x, int y, DIRECTION direction);
    std::string attack(Point p);
    bool canPlaceAirplane(Point head, DIRECTION direction, int GRID_SIZE);
};
