#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Vector3.h"

double calculateLengthFromPoint(Point pos) {
    //Exactly straight, cannot pythagoras
    if (pos.x == 0.0) return pos.z;
    if (pos.z == 0.0) return pos.x;

    return sqrt(pow(fabs(pos.x), 2) + pow(fabs(pos.z), 2));
}

/**
 * If only an end-point is provided the starting coordinates
 * of the vector are: (0, 0, 0).
 * */
Vector3::Vector3(Point end) {
    this->end = end;
}

Vector3::Vector3(Point origin, Point end) {
    this->origin = origin;
    this->end = end;
}

/**
 * This function transforms the vector to a vector with
 * length 1. Thus, the angle remains the same.
 * */
void Vector3::normalize() {
    end.x -= origin.x;
    end.y -= origin.y;
    end.z -= origin.z;

    origin = Point();
    double length = calculateLengthFromPoint(end);

    end.x /= length;
    end.y /= length;
    end.z /= length;
}

/**
 * Outputs vector data.
 * */
void Vector3::dump() {
    std::cout << "Vector dump:\n---------\n";
    origin.dump();
    end.dump();
    std::cout << "---------\n";
}