#include "./headers/main.h"

using namespace std;


int main() {
    // Create a window
    const int w = 1280, l = 720; 
    sf::RenderWindow window(sf::VideoMode(w, l), "Anchor", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 8));
    window.clear(sf::Color::Black);
    window.setFramerateLimit(60);
    sf::Clock clock;

    // Create field vars
    float x, y;
    int field_w = 320, field_l = 180; 
    int field_pos_x = w/2 - field_w/2, field_pos_y = l/2 - field_l/2;
    sf::RectangleShape field(sf::Vector2f(field_w, field_l));
    field.setFillColor(sf::Color::Transparent);
    field.setOutlineColor(sf::Color(78, 242, 143));
    field.setOutlineThickness(1);
    field.setPosition(w/2 - field.getSize().x/2, l/2 - field.getSize().y/2); 

    // Scrollbar to change the field sizes
    int value = 0;
    sf::RectangleShape scrollbar1(sf::Vector2f(17, 100));
    scrollbar1.setFillColor(sf::Color(100, 100, 100));
    scrollbar1.setPosition(50, 50);
    sf::RectangleShape slider1(sf::Vector2f(17, 20));
    slider1.setFillColor(sf::Color(200, 200, 200));
    slider1.setPosition(50, 50);
    
    // Create a the circles
    int numb_of_points = 100;
    Point_Cloud points(numb_of_points, {w/2 - field.getSize().x/2, l/2 - field.getSize().y/2}, {w/2 + field.getSize().x/2, l/2 + field.getSize().y/2}, &window);

    // Create a Matrix
    Sparse_Matrix sprs_mtrx; 
    struct Matrix mtrx = sprs_mtrx.mtrx_alloc("file.txt", 0); 
    sprs_mtrx.mtrx_print(&mtrx);
    std::vector<sf::Vector2f> outline_points = sprs_mtrx.getOutlinePoints(&mtrx);

    int i;
    for (i=0; i<outline_points.size(); ++i) printf("x: %f, y: %f\n", outline_points[i].x, outline_points[i].y);

    // Draw the ship
    Ship ship(outline_points, &window);
    ship.enlarge(54.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if(event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (slider1.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        // Clicked on the slider, enable dragging
                        bool isDragging = true;

                        while(isDragging) {
                            sf::Event dragEvent;
                                while (window.pollEvent(dragEvent)) {
                                    if (dragEvent.type == sf::Event::MouseButtonReleased &&
                                        dragEvent.mouseButton.button == sf::Mouse::Left) {
                                        // Released the mouse button, stop dragging
                                        isDragging = false;
                                } else if (dragEvent.type == sf::Event::MouseMoved) {
                                    // Update the slider position based on mouse movement
                                    float mouseY = static_cast<float>(dragEvent.mouseMove.y);
                                    float posY = std::max(50.f, std::min(150.f, mouseY));
                                    slider1.setPosition(50, posY);

                                    // Update the value based on the slider position
                                    value = (100.f * (posY - 50) / 100.f);
                                    //ship.enlarge(30.f);

                                    x = min(float(w), field_w + 1189.f * (value / 100.f));
                                    y = min(float(l), field_l + 706.f * (value / 100.f));
                                    field.setSize({x, y});
                                    field.setPosition(w/2 - field.getSize().x/2, l/2 - field.getSize().y/2); 

                                    // Print the current value to the console
                                    // std::cout << "Current Value: " << value << std::endl;
                                }
                            }

                            window.clear(sf::Color::Black);
                            window.draw(scrollbar1);
                            window.draw(slider1);
                            window.draw(field);
                            // ship.Draw(w/2, l/2);
                            window.display();
                        }
                    }
                }
            } 

            // Check for keyboard input to move the circle
            /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                circlePosition.x -= 5; // Move left
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                circlePosition.x += 5; // Move right
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                circlePosition.y -= 5; // Move up
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                circlePosition.y += 5; // Move down
            }*/
        }

        sf::Time deltaTime = clock.restart();
        float deltaTimeSeconds = deltaTime.asSeconds(); 

        // Clear the window
        window.clear(sf::Color::Black);

        // Field 
        window.draw(field);
        
        // Draw Ship
        // ship.Draw(w/2, l/2);

        // Draw circles
        points.SimulateStep(ship.getPoints(), {w/2 - field.getSize().x/2, l/2 - field.getSize().y/2}, {w/2 + field.getSize().x/2, l/2 + field.getSize().y/2}, deltaTimeSeconds);

        // Scrollbar 
        window.draw(scrollbar1);
        window.draw(slider1);

        // Display the content
        window.display();
    }

    points.~Point_Cloud();
    return 0;
}
