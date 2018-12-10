#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include "ArmSegment.h"

ArmSegment::ArmSegment() = default;

ArmSegment::ArmSegment(double length) : length(length) {}

ArmSegment::ArmSegment(double length, double angle) : length(length), angle_ground(angle) {}

ArmSegment::ArmSegment(const Point &origin, double length, double angle) : origin(origin), length(length),
                                                                           angle_ground(angle) {}
ArmSegment::ArmSegment(const Point &origin, double length) : origin(origin), length(length) {}

void ArmSegment::registerChildArmSegment(ArmSegment *armSegment) {
    this->childArmSegment = armSegment;
}

void ArmSegment::update(double angleTranslation) {
    update(nullptr, angleTranslation);
}

void ArmSegment::update(Point *newOrigin, double angleTranslation) {
    if (newOrigin == nullptr) {
        //Apply constraints
        if ((angle_joint + angleTranslation) < negativeConstraint) {
            angleTranslation = negativeConstraint + fabs(this->angle_joint);
        } else if ((angle_joint + angleTranslation) > positiveConstraint) {
            angleTranslation = positiveConstraint - fabs(this->angle_joint);
        }

        //Update joint angle (can only be done if we are the origin of a rotation)
        angle_joint += angleTranslation;
    }

    //Update ground angle
    angle_ground += angleTranslation;

    //Update new position, if required
    if (newOrigin != nullptr) {
        origin = *newOrigin;
    }

    //Update end-point coordinates based on new angle and start-point
    calculateNewEndPoint();

    //Update child armSegments, if any
    if (childArmSegment != nullptr) {
        childArmSegment->update(&end, angleTranslation);
    }
}

void ArmSegment::calculateNewEndPoint() {
    double x_translate = cos(angle_ground) * length;
    double z_translate = sin(angle_ground) * length;

    end.x = origin.x + x_translate;
    end.z = origin.z + z_translate;
}

/**
 * Outputs armSegment data.
 * */
void ArmSegment::dump() {
    std::cout << "==================\nAngle joint: " << angle_joint << "\tAngle ground: " << angle_ground << std::endl;
    std::cout << "x: " << origin.x << ", y: " << origin.y << ", z: " << origin.z << std::endl;
    std::cout << "x: " << end.x << ", y: " << end.y << ", z: " << end.z << std::endl;
    std::cout << "==================\n";
}

/// Draw segment and joint
void ArmSegment::draw(sf::RenderWindow *window){
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f(static_cast<float>(origin.x),
                                            static_cast<float>(this->origin.z)), sf::Color::Green),
                    sf::Vertex(sf::Vector2f(static_cast<float>(end.x),
                                            static_cast<float>(this->end.z)), sf::Color::Magenta)
            };

    sf::CircleShape joint(4);
    joint.setPosition(static_cast<float>(end.x - 4),
                      static_cast<float>(end.z - 4));

    window->draw(line, 2, sf::Lines);
    window->draw(joint);
}
