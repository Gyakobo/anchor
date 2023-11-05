#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>

sf::CircleShape* createCircle(float radius, sf::Vector2f position, sf::Color fillColor) {
	sf::CircleShape* circles = new sf::CircleShape[3];

    for (int i = 0; i < 3; ++i) {
        circles[i].setRadius(radius);
        circles[i].setPosition(position.x + i * 2 * radius, position.y);
        circles[i].setFillColor(fillColor);
    }

    return circles;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 200), "SFML Dynamic Circle Example",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Create the circle using the function
    sf::CircleShape* dynamicCircle = createCircle(30.f, sf::Vector2f(200.f, 100.f), sf::Color::Blue);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
		
		int i;
		for (i=0; i<3; ++i)
        	window.draw(dynamicCircle[i]);  // Draw the circle
        
		window.display();
    }

    // Don't forget to clean up the allocated memory
    delete dynamicCircle;

    return 0;
}
