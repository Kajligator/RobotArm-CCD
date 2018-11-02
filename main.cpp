//
// Created by Geert Lens on 20/10/2018.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include "arm.hpp"

#define NUM_SEG 10
#define SEGMENT_LEN 50
#define HEIGHT 1200
#define WIDTH 1200

float x, y;
bool mouse_clicked = false;

Arm arm(NUM_SEG, SEGMENT_LEN);

/// Update the coordinates of the cursor
void updateCursorPosition(sf::RenderWindow *window){
    x = sf::Mouse::getPosition(*window).x;
    y = sf::Mouse::getPosition(*window).y;
    printf("Cursor position: (%f, %f)\n", x, y);
    printf("Target: (%f, %f)\n", x - WIDTH/2.0, -(y - HEIGHT/2.0));
}

/// Draw the cursor in the window
void drawCursor(sf::RenderWindow *window){
    sf::CircleShape circle(5);
    circle.setPosition(sf::Vector2f(x-5, y-5));
    circle.setFillColor(sf::Color::Red);
    window->draw(circle);
}

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Arm Simulator");

    arm.printCoords();

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event{};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            else if( event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left )
                mouse_clicked = true;
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        if(mouse_clicked){
            printf("%s", "CLICK\n");
            updateCursorPosition(&window);
            arm.updateIK(Eigen::Vector3d(x - WIDTH/2.0, y - HEIGHT/2.0, 0));
            //arm.updateIK(Eigen::Vector3d(x, y, 0));
            arm.printCoords();
            mouse_clicked = false;
        }

        arm.drawArm(&window, WIDTH, HEIGHT);
        drawCursor(&window);

        // end the current frame
        window.display();
    }

    return 0;
}