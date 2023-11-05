#include "../headers/point_cloud.h"

float magnitude (sf::Vector2f v1, sf::Vector2f v2) {
    return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

Point_Cloud::Point_Cloud(int numb_of_points, sf::Vector2f b1, sf::Vector2f b2, sf::RenderWindow *window) {
    std::srand((unsigned) time(NULL));
    this->numb_of_points = numb_of_points;
    this->window = window;

    // Allocate memory with malloc
    circles     = new sf::CircleShape[this->numb_of_points];     
    densities   = (float*)malloc(numb_of_points * sizeof(float));
    velocities  = (sf::Vector2f*)malloc(numb_of_points * sizeof(sf::Vector2f));
    positions   = (sf::Vector2f*)malloc(numb_of_points * sizeof(sf::Vector2f));
    predictedPositions = (sf::Vector2f*)malloc(numb_of_points * sizeof(sf::Vector2f));

    #pragma omp parallel
    {
        int id = omp_get_thread_num();  // Get thread number
        int n = omp_get_num_threads();   // Get number of threads
        int i;

        for (i=id; i<numb_of_points; i+=n) {
            float x = std::rand() % (int(b2.x - b1.x) + 1) + int(b1.x);
            float y = std::rand() % (int(b2.y - b1.y) + 1) + int(b1.y);
            circles[i].setRadius(size);
            circles[i].setFillColor(sf::Color(66, 135, 245));  

            positions[i]    = sf::Vector2f(x, y);
            densities[i]    = CalculateDensity(positions[i]);
            velocities[i]   = sf::Vector2f(0, 0);
        }
    }

}

Point_Cloud::~Point_Cloud() { 
    free(circles);
    free(densities);
    free(velocities);
    free(positions);
}


float Point_Cloud::SmoothingKernel(float dst, float r) {
    if (dst >= r) return 0;

    float volume = M_PI * pow(r, 4) / 6;
    return (r-dst) * (r-dst)/volume;
}

float Point_Cloud::SmoothingKernelDerivative(float dst, float r) {
    if (dst >= r) return 0;

    float scale = 12/(pow(r, 4) * M_PI);
    return (dst - r) * scale;
}

float Point_Cloud::CalculateDensity(sf::Vector2f samplePoint) {
    float density = 0;

    int i;
    for (i=0; i<numb_of_points; ++i) {
        float dst = magnitude(positions[i], samplePoint); 
        float influence = SmoothingKernel(dst, smoothingRadius); 
        density += mass * influence;
    } 

    return density;
}

float Point_Cloud::ConvertDensityToPressure(float density) {
    float densityError = density - targetDensity;
    float pressure = densityError * pressureMultiplier;
    return pressure;
}

float Point_Cloud::CalculateSharedPressure(float densityA, float densityB) {
    float pressureA = ConvertDensityToPressure(densityA);
    float pressureB = ConvertDensityToPressure(densityB);
    return (pressureA + pressureB) / 2;
}

sf::Vector2f Point_Cloud::CalculatePressureForce(int particleIndex) {
    sf::Vector2f pressureForce = sf::Vector2f(0, 0);

    int i;
    for (i=0; i<numb_of_points; ++i) {
        if (particleIndex == i) continue;

        sf::Vector2f offset = positions[i] - positions[particleIndex];
        float dst = magnitude(offset, sf::Vector2f(0, 0)); 

        sf::Vector2f dir; 
        if (dst == 0) dir = sf::Vector2f(0, -1);
        else dir = offset / dst;

        float slope = SmoothingKernelDerivative(dst, smoothingRadius);
        float density = densities[i]; // Still have to instantiate
        // pressureForce += -ConvertDensityToPressure(density) * dir * slope * mass / density; 
        float sharedPressure = CalculateSharedPressure(density, densities[particleIndex]);
        pressureForce += sharedPressure * dir * slope * mass / density; 
    }

    return pressureForce;
}

void Point_Cloud::SimulateStep(sf::Vector2f b1, sf::Vector2f b2, float deltaTime) {
    // Apply gravity and calculate densities
    #pragma omp parallel
    {
        int id = omp_get_thread_num();  // Get thread number
        int n = omp_get_num_threads();   // Get number of threads
        int i;

        for (i=id; i<numb_of_points; i+=n) {
            velocities[i] += (sf::Vector2f(0, 1) * gravity * deltaTime);
            // predictedPositions[i] = positions[i] + velocities[i] * deltaTime;
            densities[i] = CalculateDensity(positions[i]);
        }
    }
    
    // delete me
    /*#pragma omp parallel
    {
        int id = omp_get_thread_num();  // Get thread number
        int n = omp_get_num_threads();   // Get number of threads
        int i;
        
        for (i=id; i<numb_of_points; i+=n) {
            densities[i] = CalculateDensity(predictedPositions[i]);
        }
    }*/


    // Caculate and apply pressure forces
    #pragma omp parallel
    {
        int id = omp_get_thread_num();  // Get thread number
        int n = omp_get_num_threads();   // Get number of threads
        int i;
        
        for (i=id; i<numb_of_points; i+=n) {
            sf::Vector2f pressureForce = CalculatePressureForce(i);
            sf::Vector2f pressureAcelleration = pressureForce / densities[i];
            velocities[i] += pressureAcelleration * deltaTime;
        }
    }


    // Update positions and resolve collisions 
    #pragma omp parallel
    {
        int id = omp_get_thread_num();  // Get thread number
        int n = omp_get_num_threads();   // Get number of threads
        int i;

        for (i=id; i<numb_of_points; i+=n) {
            positions[i] += velocities[i] * deltaTime;
            
            if (positions[i].x >= b2.x - size*2) {
                velocities[i].x *= -1 * collisionDamping;
                positions[i].x = b2.x - size*2;
            }
            else if (positions[i].x < b1.x + size*2) {
                velocities[i].x *= -1 * collisionDamping;
                positions[i].x = b1.x + size*2;
            } 
            
            if (positions[i].y >= b2.y - size*2) {
                velocities[i].y *= -1 * collisionDamping;
                positions[i].y = b2.y - size*2;
            }
            else if (positions[i].y < b1.y + size*2) {
                velocities[i].y *= -1 * collisionDamping;
                positions[i].y = b1.y + size*2;
            } 
        }
    }
    
    int i;
    for (i=0; i<numb_of_points; ++i) {
        circles[i].setPosition(positions[i]);
        window->draw(circles[i]);
    }
    
}






