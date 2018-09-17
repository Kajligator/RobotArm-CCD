#include "segment.hpp"
#include <cmath>

Segment::Segment() = default;

Segment::Segment(float x, float y, float len, float angle){
    this->a = Vector2(x, y);
    this->len = len;
    this->angle = angle;
    calculateB();
}

Segment::Segment(Segment *parent, float len, float angle) {
    this->parent = parent;
    this->a = parent->b;
    this->len = len;
    this->angle = angle;
    calculateB();
}

void Segment::calculateB() {
    float dx = len * cos(this->angle);
    float dy = len * sin(this->angle);
    this->b.set(this->a.x + dx, this->a.y + dy);
}

void Segment::update(){
    calculateB();
}

Vector2 Segment::getA(){
    return a;
}

Vector2 Segment::getB(){
    return b;
}

void Segment::setA(Vector2 *pos) {
    this->a.set(pos->x, pos->y);
}

void Segment::draw(sf::RenderWindow *window){
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f(this->a.x, this->a.x)),
                    sf::Vertex(sf::Vector2f(this->b.x, this->b.y))
            };
    window->draw(line, 2, sf::Lines);
}