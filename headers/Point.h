#ifndef POINT_H
#define POINT_H

#include <SFML/Graphics.hpp>

class Point {
public:
    Point(Vector2f pos);
    void update();
    void resolveCollisions(sf::Vector2f b1, sf::Vector2f b2);

private:
    float gravity;
    sf::Vector2f pos;
    sf::Vector2f velocity;
}

#endif