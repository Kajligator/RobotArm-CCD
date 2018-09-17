#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include "segment.hpp"

#define NUM_SEG 3

float len = 50;

float toRadians(float degrees) {
    return static_cast<float>(degrees * M_PI / 180.0);
}

float toDegrees(float radians) {
    return static_cast<float>(radians * (180.0 / M_PI));
}

int main() {

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Arm Simulator");

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event{};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // end the current frame
        window.display();
    }

    return 0;
}