#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <cmath> 
#include <ctime> 
#include <omp.h>
// #include "Point.h"

using namespace std;

class Point_Cloud {
public:
    Point_Cloud(int numb_of_points, sf::Vector2f b1, sf::Vector2f b2, sf::RenderWindow *window);
    ~Point_Cloud();
    void SimulateStep(std::vector<sf::Vector2f> polygon, sf::Vector2f b1, sf::Vector2f b2, float deltaTime);

private:
    // Gravity
    int size = 5; 
    float gravity           = 1.2f;
    float mass              = 1.6f;
    float collisionDamping  = 0.8f;

    // Pressure
    int numb_of_points;
    float smoothingRadius = 45.35f; // 45.35f
    float targetDensity = 1.2f;
    float pressureMultiplier = -600.f; // -600
    
    // Arrays 
    float *densities;
    sf::Vector2f *velocities;
    sf::Vector2f *positions;
    sf::Vector2f *predictedPositions;
    sf::CircleShape *circles; 

    sf::RenderWindow *window;
    float SmoothingKernel(float dst, float r);
    float SmoothingKernelDerivative(float dst, float r);
    float CalculateDensity(sf::Vector2f samplePoint);
    float CalculateSharedPressure(float densityA, float densityB);

    float ConvertDensityToPressure(float density);

    sf::Vector2f CalculatePressureForce(int particleIndex);
};

#endif