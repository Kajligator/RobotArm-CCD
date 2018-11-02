//
// Created by Geert Lens on 20/10/2018.
//

#include "segment.hpp"
#include <cmath>
#include <SFML/Graphics.hpp>

segment::segment(){
    this->begin = Eigen::Vector3d(0, 0, 0);
    this->end = Eigen::Vector3d(0, 0, 0);
}

segment::segment(Eigen::Vector3d end, double len) {
    this->begin = end;
    this->length = len;
    calculateEndPoint(begin, angle);
    this->angle = 0;
}

/// Calculate end point of segment with given angle
void segment::calculateEndPoint(Eigen::Vector3d bp, double angle) {
    double y = (length * sin(angle)) + bp.y();
    double x = (length * cos(angle)) + bp.x();
    this->end = Eigen::Vector3d(x, y, 0);
    this->angle = angle;
}

/// Calculate end point of segment with stored values
void segment::calculateEndPoint() {
    double dx = length * cos(this->angle);
    double dy = length * sin(this->angle);
    this->end = Eigen::Vector3d(this->begin.x() + dx, this->begin.y() + dy, 0);
}

/// Draw segment and joint
void segment::draw(sf::RenderWindow *window, float screen_width, float screen_height){
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f(this->begin.x()+screen_width/2, this->begin.y()+screen_height/2), sf::Color::Green),
                    sf::Vertex(sf::Vector2f(this->end.x()+screen_width/2, this->end.y()+screen_height/2), sf::Color::Magenta)
            };

    sf::CircleShape joint(4);
    joint.setPosition(end.x()-4+screen_width/2, end.y()-4+screen_height/2);

    window->draw(line, 2, sf::Lines);
    window->draw(joint);
}

Eigen::Vector3d segment::getBegin() {
    return this->begin;
}

Eigen::Vector3d segment::getEnd() {
    return this->end;
}

double segment::getAngle() {
    return this->angle;
}

void segment::setBegin(Eigen::Vector3d begin) {
    this->begin = begin;
}