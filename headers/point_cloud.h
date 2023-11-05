#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <cmath> 
#include <ctime> 
#include "Point.h"

using namespace std;

class Point_Cloud {
public:
    Point_Cloud(int numb_of_points, sf::Vector2f b1, sf::Vector2f b2, sf::RenderWindow *window);
    ~Point_Cloud();
    void Draw(sf::Vector2f b1, sf::Vector2f b2, float deltaTime); // Args are the boundaries

private:
    int numb_of_points;
    float smoothingRadius = 1.f;
    float targetDensity;
    float pressureMultiplier;

    sf::RenderWindow *window;
    float SmoothingKernel(float dst, float r);
    float SmoothingKernelDerivative(float dst, float r);
    float CalculateDensity(sf::Vector2f samplePoint);
    
    float ConvertDensityToPressure(float density);

    Point *points;
    sf::CircleShape *circles; 

    // sf::Vector2f CalculatePropertyGradient(sf::Vector2f samplePoint);
    // float CalculateProperty(sf::Vector2f samplePoint);
};

float magnitude (sf::Vector2f v1, sf::Vector2f v2) {
    return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

float Pow(float base, int exp) {
    int i;
    float res = 1; 
    for (i=0; i<exp; ++exp) {
        res *= base;
    }
    return res;
}

#endif