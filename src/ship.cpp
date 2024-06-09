#include "../headers/ship.h"

Ship::Ship(std::vector<sf::Vector2f> points, sf::RenderWindow *window) {
    this->window = window;
    this->points = points;
}

void Ship::enlarge(float s) {
    size_t i;
    for (i = 0; i < points.size(); ++i) {
        pos += points[i]; 
    }
    pos /= float(points.size());
    
    for (i = 0; i < points.size(); ++i) {
        points[i].x = pos.x + (points[i].x - pos.x) * s; 
        points[i].y = pos.y + (points[i].y - pos.y) * s; 
    }
}

void Ship::Draw(int x, int y) {
    size_t i;

    for (i = 0; i < points.size() - 1; ++i) {
        sf::Vertex line[] = {
            sf::Vertex(points[i] + sf::Vector2f(x, y), sf::Color(252, 124, 25)),
            sf::Vertex(points[i + 1] + sf::Vector2f(x, y), sf::Color(252, 124, 25))
        };

        window->draw(line, 2, sf::Lines);
    }
    
    sf::Vertex line[] = {
        sf::Vertex(points[i] + sf::Vector2f(x, y), sf::Color(252, 124, 25)),
        sf::Vertex(points[0] + sf::Vector2f(x, y), sf::Color(252, 124, 25))
    };

    window->draw(line, 2, sf::Lines);
}

