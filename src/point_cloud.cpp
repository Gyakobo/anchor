#include "../headers/point_cloud.h"

Point_Cloud::Point_Cloud(int numb_of_points, sf::Vector2f b1, sf::Vector2f b2, sf::RenderWindow *window) {
    //std::srand(NULL);
    this->numb_of_points = numb_of_points;
    this->window = window;

    // Allocate memory with malloc
    points = (Point*) malloc(numb_of_points * sizeof(Point));     
    circles = (sf::CircleShape*) malloc(numb_of_points * sizeof(sf::CircleShape));     
    
    int i;
    for (i=0; i<numb_of_points; ++i) {
        // int x = std::rand() % (int(b2.x - b1.x) + 1) + int(b1.x);
        // int y = std::rand() % (int(b2.y - b1.y) + 1) + int(b1.y);
        //points[i] = Point({x, y}, 5);
        points[i] = Point({b1.x + 20, b1.y + 20}, 5);
        circles[i] = sf::CircleShape(5);
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
        // points[i].update(deltaTime);
        // points[i].resolveCollisions(b1, b2);
        //circles[i].setPosition(points[i].getPos()); 
        //window->draw(circles[i]);
    }

}



float Point_Cloud::SmoothingKernel(float r, float dst) {
    float val = std::max(0.f, r * r - dst * dst);
    return val * val * val;
}

float Point_Cloud::CalculateDensity(sf::Vector2f samplePoint) {
    float density = 0;
    const float mass = 1;

    return density;
}




