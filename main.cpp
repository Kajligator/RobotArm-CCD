//
// Created by Geert Lens on 20/10/2018.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fenv.h>

#include "ArmSegment.h"
#include "Ccd.h"

#define HEIGHT 1200
#define WIDTH 1200
#define VERBOSE

//Default target to reach
#define DEFAULT_TARGET_X 680.0
#define DEFAULT_TARGET_Y 550.0

//Default arm constants
#define NUMBER_OF_ARM_SEGMENTS 3
#define DEFAULT_ARM_LENGTH 50.0
#define ARM_STARTING_ANGLE 0.0

//Default constraints for armSegments
#define DEFAULT_CONSTRAINT 1.396263
#define DEFAULT_DAMPING 0.1

bool mouse_clicked = false;
bool success = false;

Point pointToReach;

/// Update the coordinates of the cursor
void updateCursorPosition(sf::RenderWindow *window){
    pointToReach = Point(sf::Mouse::getPosition(*window).x, 0.0, sf::Mouse::getPosition(*window).y);
#ifdef VERBOSE
    pointToReach.dump();
#endif
}

/// Draw the cursor in the window
void drawCursor(sf::RenderWindow *window){
    sf::CircleShape circle(5);
    circle.setPosition(sf::Vector2f(static_cast<float>(pointToReach.x - 5), static_cast<float>(pointToReach.z - 5)));
    circle.setFillColor(sf::Color::Red);
    window->draw(circle);
}

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Arm Simulator");

    // Set default target values
    double target_x = DEFAULT_TARGET_X;
    double target_y = DEFAULT_TARGET_Y;

    // Set target
    pointToReach = Point(target_x, 0.0, target_y);
    std::cout << "Will try to reach point x: " << target_x << ", y: " << target_y << "\nApplying a damping of: "
              << DEFAULT_DAMPING << " radians, and setting constraints per joint (skipping base) to "
              << DEFAULT_CONSTRAINT << " radians" << std::endl;


    // Create arm segments
    auto *armSegments = new ArmSegment[NUMBER_OF_ARM_SEGMENTS];

    for (int i = 0; i < NUMBER_OF_ARM_SEGMENTS; ++i) {
        armSegments[i] = ArmSegment(DEFAULT_ARM_LENGTH);

        //Set damping for more fluent movements
        armSegments[i].damping = DEFAULT_DAMPING;

        if (i != 0) {
            // Set joint constraints for all arm segments 45 degrees, but skip the base arm segment (seemed more natural)
            armSegments[i].negativeConstraint = -DEFAULT_CONSTRAINT;
            armSegments[i].positiveConstraint = DEFAULT_CONSTRAINT;
        }

        // Register callback, used to iterate updates
        if (i > 0) armSegments[i - 1].registerChildArmSegment(&armSegments[i]);
    }

    Point ground = Point(WIDTH/2, 0.0, HEIGHT/2);
    armSegments[0].update(&ground, ARM_STARTING_ANGLE);

#ifdef VERBOSE
    std::cout << "\nStarting positions dump (per armSegment): \n";
    for (int i = 0; i < NUMBER_OF_ARM_SEGMENTS; ++i) {
        armSegments[i].dump();
    }
#endif

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
#ifdef VERBOSE
            std::cout << "CLICK\n";
#endif
            updateCursorPosition(&window);
            // Apply Cyclic Coordinate Descent
            Ccd ccd = Ccd(armSegments, NUMBER_OF_ARM_SEGMENTS);
            success = ccd.computeCcdLink(pointToReach);

#ifdef VERBOSE
            std::cout << "\nEnd positions dump (per armSegment): \n";
            std::vector<double> x_arm_points;
            std::vector<double> y_arm_points;
            x_arm_points.push_back(0.0);
            y_arm_points.push_back(0.0);

            for (int i = 0; i < NUMBER_OF_ARM_SEGMENTS; ++i) {
                x_arm_points.push_back(armSegments[i].end.x);
                y_arm_points.push_back(armSegments[i].end.z);
                armSegments[i].dump();
            }
#endif
            // Show whether the target was reached
            std::cout << "The target was reached: " << (success ? "True" : "False") << "\n";
            mouse_clicked = false;
        }

        drawCursor(&window);

        for(int i = 0; i < NUMBER_OF_ARM_SEGMENTS; ++i){
            armSegments[i].draw(&window);
        }

        // end the current frame
        window.display();
    }

    return 0;
}