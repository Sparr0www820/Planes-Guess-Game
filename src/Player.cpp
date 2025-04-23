#include "Player.hpp"

Player::Player() : grid(15, std::vector<int>(15, -1)) {}

void Player::placeAirplane(int x, int y, DIRECTION direction) {
    airplanes.emplace_back(x, y, direction);
}

std::string Player::attack(Point p) {
    for (auto& airplane : airplanes) {
        if (airplane.isHead(p)) {
            planesDown++;
            return "Head hit!";
        } else if (airplane.isBody(p)) {
            return "Body hit!";
        }
    }
    return "Miss!";
}

bool Player::canPlaceAirplane(Point head, DIRECTION direction, int GRID_SIZE) {
    Airplane airplane(head.x, head.y, direction);
    for (const auto& otherPlane : airplanes) {
        if (otherPlane.isHead(head) || otherPlane.isBody(head)) {
            return false;
        }
    }
    for (const auto& part : airplane.body) {
        if (part.x < 0 || part.x >= GRID_SIZE || part.y < 0 || part.y >= GRID_SIZE) {
            return false;
        }
        for (const auto& otherPlane : airplanes) {
            if (otherPlane.isHead(part) || otherPlane.isBody(part)) {
                return false;
            }
        }
    }
    return true;
}
