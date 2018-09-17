#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include "segment.hpp"

#define NUM_SEG 3
#define HEIGHT 800
#define WIDTH 1200
float len = 100;
Segment segments[] = {};

float toRadians(float degrees) {
    return static_cast<float>(degrees * M_PI / 180.0);
}

float toDegrees(float radians) {
    return static_cast<float>(radians * (180.0 / M_PI));
}

int main() {

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Arm Simulator");

    segments[0] = Segment(WIDTH/2, HEIGHT, len, 0);

    for(int i = 1; i<NUM_SEG; i++){
        segments[i] = Segment(segments[i-1], len, -1);
        segments[i].update();
    }

    for(int i = 0; i<NUM_SEG; i++){
        printf("Segment %i: A:(%f, %f), B:(%f, %f)\n", i, segments[i].getA().x, segments[i].getA().y, segments[i].getB().x, segments[i].getB().y);
        segments[i].draw(&window);
    }

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

        Vector2 mousePos = Vector2(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

        segments[NUM_SEG-1].setB(&mousePos);

        for(int i = 0; i<NUM_SEG; i++){
            segments[i].draw(&window);
        }

        // end the current frame
        window.display();
    }

    return 0;
}