#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <iostream>
#include <unordered_map>

constexpr unsigned WINDOW_WIDTH  = 1350u;
constexpr unsigned WINDOW_HEIGHT = 800u;

constexpr int GRID_SIZE   = 15;
constexpr int CELL_SIZE   = 40;
constexpr int PLANE_COUNT = 3;

void drawGrid(sf::RenderWindow& window, const int              offsetX, const int offsetY, const Player& player,
              const bool        highlight = false, const Point highlightPoint = Point()) {
    sf::RectangleShape cell(sf::Vector2f({ CELL_SIZE - 2, CELL_SIZE - 2 }));
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            cell.setPosition({
                static_cast<float>(offsetX + j * CELL_SIZE), static_cast<float>(offsetY + i * CELL_SIZE)
            });

            if (player.grid[i][j] == 1) {
                cell.setFillColor(sf::Color::Yellow); // Hit body
            } else if (player.grid[i][j] == 2) {
                cell.setFillColor(sf::Color::Red); // Hit head
            } else if (player.grid[i][j] == 0) {
                cell.setFillColor(sf::Color(155, 155, 155));
            } else if (highlight && i == highlightPoint.x && j == highlightPoint.y) {
                //cell.setFillColor(sf::Color::Green);  // Highlighted position for placement
            } else {
                cell.setFillColor(sf::Color::White); // Empty cell
            }

            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            window.draw(cell);
        }
    }

    // Draw planes on the grid during the placement phase (phase 1)
    if (highlight && !player.airplanes.empty()) {
        for (const auto& airplane : player.airplanes) {
            // Draw the body parts (body and head)
            for (const auto& part : airplane.body) {
                cell.setPosition({
                    static_cast<float>(offsetX + part.y * CELL_SIZE), static_cast<float>(offsetY + part.x * CELL_SIZE)
                });
                cell.setFillColor(sf::Color::Blue); // Body parts
                window.draw(cell);
            }
            cell.setPosition({
                static_cast<float>(offsetX + airplane.head.y * CELL_SIZE),
                static_cast<float>(offsetY + airplane.head.x * CELL_SIZE)
            });
            cell.setFillColor(sf::Color::Magenta); // Head part
            window.draw(cell);
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Planes Guess Game");

    Player                         player1, player2;
    int                            currentPlayer = 1; // 1: Player 1, 2: Player 2
    int                            phase         = 1; // 1: Placement phase, 2: Attack phase
    int                            placedPlanes  = 0; // Tracks the number of planes placed by the current player
    std::string                    status        = "Player 1 is placing airplanes...";
    std::vector<std::vector<bool>> attackHistory1(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
    // Player 1's attack history
    std::vector<std::vector<bool>> attackHistory2(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
    // Player 2's attack history
    DIRECTION currentOrientation = UP; // Default orientation for airplane placement

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::MouseWheelScrolled>()) {
                if (event->getIf<sf::Event::MouseWheelScrolled>()->delta > 0) {
                    currentOrientation = static_cast<DIRECTION>((currentOrientation + 1) % 4); // Rotate clockwise
                } else {
                    currentOrientation = static_cast<DIRECTION>((currentOrientation + 3) % 4);
                    // Rotate counterclockwise
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button ==
                sf::Mouse::Button::Left) {
                auto mouse = event->getIf<sf::Event::MouseButtonPressed>();
                int  gridX, gridY;

                if (currentPlayer == 1) {
                    // For Player 1 (left grid)
                    gridX = (mouse->position.x - 50) / CELL_SIZE;
                    gridY = (mouse->position.y - 50) / CELL_SIZE;
                } else {
                    // For Player 2 (right grid)
                    gridX = (mouse->position.x - 700) / CELL_SIZE;
                    gridY = (mouse->position.y - 50) / CELL_SIZE;
                }

                if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
                    Point selectedPoint(gridY, gridX);

                    if (phase == 1) {
                        // Placement phase
                        Player& currentPlayerRef = (currentPlayer == 1) ? player1 : player2;
                        bool    isValid          = true;

                        // Check if any part of the airplane overlaps or goes out of bounds
                        if (!currentPlayerRef.canPlaceAirplane(selectedPoint, currentOrientation, GRID_SIZE)) {
                            isValid = false;
                        }

                        if (isValid) {
                            currentPlayerRef.placeAirplane(selectedPoint.x, selectedPoint.y, currentOrientation);
                            placedPlanes++;

                            if (placedPlanes == PLANE_COUNT) {
                                if (currentPlayer == 1) {
                                    currentPlayer = 2;
                                    placedPlanes  = 0;
                                    status        = "Player 2 is placing airplanes...";
                                } else {
                                    currentPlayer = 1;
                                    phase         = 2;
                                    status        = "Player 1's turn to attack!";
                                }
                            }
                        } else {
                            status = "Invalid position, try again!";
                        }
                    } else if (phase == 2) {
                        // Attack phase
                        Player& currentPlayerRef = (currentPlayer == 1) ? player1 : player2;
                        Player& targetPlayerRef  = (currentPlayer == 1) ? player2 : player1;

                        // Prevent repeated attacks on the same cell
                        if ((currentPlayer == 1 && attackHistory1[selectedPoint.x][selectedPoint.y]) ||
                            (currentPlayer == 2 && attackHistory2[selectedPoint.x][selectedPoint.y])) {
                            status = "You already attacked this position!";
                        } else {
                            std::string attackResult = targetPlayerRef.attack(selectedPoint);

                            // Update attack history
                            if (currentPlayer == 1) {
                                attackHistory1[selectedPoint.x][selectedPoint.y] = true;
                            } else {
                                attackHistory2[selectedPoint.x][selectedPoint.y] = true;
                            }

                            if (attackResult == "Head hit!") {
                                status = "Player " + std::to_string(currentPlayer) + " hit a plane head!";

                                currentPlayerRef.grid[selectedPoint.x][selectedPoint.y] = 2;
                            } else if (attackResult == "Body hit!") {
                                status = "Player " + std::to_string(currentPlayer) + " hit a plane body!";

                                currentPlayerRef.grid[selectedPoint.x][selectedPoint.y] = 1;
                            } else {
                                status = "Miss!";

                                currentPlayerRef.grid[selectedPoint.x][selectedPoint.y] = 0;
                            }

                            // Check if the game is over
                            if (player1.planesDown == PLANE_COUNT) {
                                status = "Player 2 wins!";
                                break;
                            } else if (player2.planesDown == 3) {
                                status = "Player 1 wins!";
                                break;
                            }

                            // Switch turn
                            currentPlayer = (currentPlayer == 1) ? 2 : 1;
                            status += " Player " + std::to_string(currentPlayer) + "'s turn to attack!";
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        // Draw Player 1's map (left side) and Player 2's map (right side)
        drawGrid(window, 50, 50, player1, phase == 1 && currentPlayer == 1);  // Player 1's grid (left)
        drawGrid(window, 700, 50, player2, phase == 1 && currentPlayer == 2); // Player 2's grid (right)

        // Render status message
        sf::Font font;
        if (!font.openFromFile("PAPYRUS.TTF")) {
            std::cerr << "Error loading font from file!\n";
            return EXIT_FAILURE;
        }
        sf::Text textStatus(font, status, 30);
        textStatus.setFillColor(sf::Color::Black);
        textStatus.setPosition({ 50, 700 });
        window.draw(textStatus);

        if (phase == 1) {
            std::string rotationText = "Direction: ";
            if (currentOrientation == UP) {
                rotationText += "UP";
            } else if (currentOrientation == LEFT) {
                rotationText += "LEFT";
            } else if (currentOrientation == DOWN) {
                rotationText += "DOWN";
            } else if (currentOrientation == RIGHT) {
                rotationText += "RIGHT";
            }
            sf::Text text(font, rotationText, 30);
            text.setFillColor(sf::Color::Black);
            text.setPosition({ 700, 700 });
            window.draw(text);
        }

        window.display();
    }

    return 0;
}
