#include "../headers/Point.h"

Point::Point(sf::Vector2f position, int size) {
    this->position = position;
    this->size = size;
    velocity = sf::Vector2f(0, 0);
}

void Point::addToVelocity(sf::Vector2f v) {
    velocity += v;
}


void Point::resolveCollisions(sf::Vector2f b1, sf::Vector2f b2) {
    if (position.x+size*2 >= b2.x) 
        velocity.x *= -1 * collisionDamping;
    else if (position.x+size*2 < b1.x) 
        velocity.x *= -1 * collisionDamping;
    
    if (position.y+size*2 >= b2.y) 
        velocity.y *= -1 * collisionDamping;
    else if (position.y+size*2 < b1.y) 
        velocity.y *= -1 * collisionDamping;
}

sf::Vector2f Point::getPos() {
    return position;
}

