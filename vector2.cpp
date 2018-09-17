#include <numeric>
#include "vector2.hpp"

Vector2::Vector2()
{
}

Vector2::Vector2(float x, float y){
    this->x = x;
    this->y = y;
}

void Vector2::set(float x, float y){
    this->x = x;
    this->y = y;
}

Vector2 * Vector2::setMag(float len){
    normalize();
    mult(len);
    return this;
}

Vector2 * Vector2::add(Vector2& anotherVector) {
    this->x = this->x + anotherVector.x;
    this->y = this->y + anotherVector.y;
    return this;
}

Vector2 * Vector2::sub(Vector2& anotherVector) {
    this->x = this->x - anotherVector.x;
    this->y = this->y - anotherVector.y;
    return this;
}

Vector2 * Vector2::mult(float n) {
    this->x *= n;
    this->y *= n;
    return this;
}

float Vector2::dot(Vector2& anotherVector) {
    return this->x*anotherVector.x+this->y*anotherVector.y;
}

float Vector2::magnitude(){
    return sqrt((this->x*this->x) + (this->y*this->y));
}

float Vector2::angle(){
    float angle = atan2(y, x);
    return (angle);
}

Vector2 * Vector2::normalize() {
    float m = mag();
    if (m != 0 && m != 1) {
        div(m);
    }
    return this;
}

float Vector2::mag() {
    return sqrt(x*x + y*y);
}

Vector2 * Vector2::div(float n) {
    this->x /= n;
    this->y /= n;
    return this;
}
