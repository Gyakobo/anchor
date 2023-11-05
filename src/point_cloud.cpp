#include "../headers/point_cloud.h"

Point_Cloud::Point_Cloud(int numb_of_points, sf::Vector2f b1, sf::Vector2f b2, sf::RenderWindow *window) {
    std::srand((unsigned) time(NULL));
    this->numb_of_points = numb_of_points;
    this->window = window;

    // Allocate memory with malloc
    //points = new Point[this->numb_of_points];     
    points = (Point *)malloc(numb_of_points * sizeof(Point)); 
    if (!points) return;
    circles = new sf::CircleShape[this->numb_of_points];     
    
    int i;
    for (i=0; i<numb_of_points; ++i) {
        float x = std::rand() % (int(b2.x - b1.x) + 1) + int(b1.x);
        float y = std::rand() % (int(b2.y - b1.y) + 1) + int(b1.y);
        points[i] = Point({x, y}, 5);
        // points[i] = Point({b1.x + 20, b1.y + 20}, 5);
        circles[i].setRadius(5);
        circles[i].setFillColor(sf::Color(66, 135, 245));  
    }
}

Point_Cloud::~Point_Cloud() { 
    free(points);
    free(circles);
}

void Point_Cloud::Draw(sf::Vector2f b1, sf::Vector2f b2, float deltaTime) {
    int i;
    for (i=0; i<numb_of_points; ++i) {
        points[i].update(deltaTime);
        points[i].resolveCollisions(b1, b2);
        circles[i].setPosition(points[i].getPos());
        window->draw(circles[i]);
    }
}


float Point_Cloud::SmoothingKernel(float dst, float r) {
    float volume = M_PI * Pow(r, 8) / 4;
    float val = std::max(0.f, r * r - dst * dst);
    return val * val * val / volume;
}

float Point_Cloud::SmoothingKernelDerivative(float dst, float r) {
    if (dst >= r) return 0;
    float f = r * r - dst * dst;
    float scale = -24 / (M_PI * Pow(r, 8));
    return scale * dst * f * f;
}

float Point_Cloud::CalculateDensity(sf::Vector2f samplePoint) {
    float density = 0;
    const float mass = 1;

    int i;
    for (i=0; i<numb_of_points; ++i) {
        float dst = magnitude(points[i].getPos(), samplePoint); 
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

/*sf::Vector2f Point_Cloud::CalculateProperty(sf::Vector2f samplePoint) {
    float property = 0;
    int i;
    for (i=0; i<numb_of_points; ++) {
        float dst = magnitude(points[i].getPos(), samplePoint); 
        float influence = SmoothingKernel(dst, smoothingRadius);
        property += particleP
    }

    return property;
}

sf::Vector2f Point_Cloud::CalculatePropertyGradient(sf::Vector2f samplePoint) {
    const float stepSize = 0.f;
    float deltaX = C
}*/






