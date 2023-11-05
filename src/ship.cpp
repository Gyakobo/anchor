#include "../headers/ship.h"

Ship::Ship(std::vector<sf::Vector2f> points, sf::RenderWindow *window) {
    this->window = window;
    this->points = points;
}

void Ship::Draw(int x, int y, float s) {
    size_t i;
    for (i = 0; i < points.size() - 1; ++i) {
        pos += points[i] / float(points.size()); 
    }

    for (i = 0; i < points.size() - 1; ++i) {
        sf::Vertex line[] = {
            sf::Vertex(points[i] * s + sf::Vector2f(x, y) - pos, sf::Color(252, 124, 25)),
            sf::Vertex(points[i + 1] * s + sf::Vector2f(x, y) - pos, sf::Color(252, 124, 25))
        };

        window->draw(line, 10, sf::Lines);
    }
}

