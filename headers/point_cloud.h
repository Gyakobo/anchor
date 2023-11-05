#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <ctime> 
#include "Point.h"

using namespace std;

class Point_Cloud {
public:
    Point_Cloud(int numb_of_points, sf::Vector2f b1, sf::Vector2f b2, sf::RenderWindow * window);
    ~Point_Cloud();
    void Draw(sf::Vector2f b1, sf::Vector2f b2, float deltaTime); // Args are the boundaries

private:
    int numb_of_points;

    sf::RenderWindow *window;
    float SmoothingKernel(float r, float dst);
    float CalculateDensity(sf::Vector2f samplePoint);
    Point *points;
    sf::CircleShape *circles; 
};


#endif