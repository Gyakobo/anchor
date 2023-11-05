#ifndef POINT_H
#define POINT_H

#include <SFML/Graphics.hpp>

class Point {
public:
    Point(sf::Vector2f position, int size);

    void resolveCollisions(sf::Vector2f b1, sf::Vector2f b2);
    sf::Vector2f getPos();
    void addToVelocity(sf::Vector2f v);

private:
    float gravity = 28.6f;
    sf::Vector2f position;
    sf::Vector2f velocity;
    int size;
    float collisionDamping = 0.6f;
};

#endif