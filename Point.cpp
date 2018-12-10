#include <iostream>

#include "Point.h"

/**
 * Creates a point with values (0.0, 0.0, 0.0).
 * */
Point::Point() = default;

/**
 * Creates a point using the given coordinates.
 * */
Point::Point(double x, double y, double z) : x(x), y(y), z(z) {}

/**
 * Outputs point data.
 * */
void Point::dump() {
    std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
}