#ifndef SHIP_H
#define SHIP_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

class Ship {
public:
    Ship(std::vector<sf::Vector2f> points, sf::RenderWindow *window);
    void Draw(int x, int y);
    void enlarge(float s);
    std::vector<sf::Vector2f> getPoints() {
        return points;
    }

private:
    sf::Vector2f pos = sf::Vector2f(0, 0);

    sf::RenderWindow* window;
    std::vector<sf::Vector2f> points;
};

#endif