//
// Created by Geert Lens on 20/10/2018.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fenv.h>

#include "CCD/ArmSegment.h"
#include "CCD/Ccd.h"

#define VERBOSE

//Default target to reach
#define DEFAULT_TARGET_X 200.0
#define DEFAULT_TARGET_Y 50.0

//Default arm constants
#define NUMBER_OF_ARM_SEGMENTS 3
#define ARM_STARTING_ANGLE 0.0

//Default constraints for armSegments
#define DEFAULT_CONSTRAINT 1.570796
#define DEFAULT_DAMPING 1

bool success = false;

int main() {

    // Set default target values
    double target_x = DEFAULT_TARGET_X;
    double target_y = DEFAULT_TARGET_Y;

    // Set target
    Point pointToReach = Point(target_x, 0.0, target_y);
    std::cout << "Will try to reach point x: " << target_x << ", y: " << target_y << "\nApplying a damping of: "
              << DEFAULT_DAMPING << " radians, and setting constraints per joint (skipping base) to "
              << DEFAULT_CONSTRAINT << " radians" << std::endl;


    // Create arm segments
    auto *armSegments = new ArmSegment[NUMBER_OF_ARM_SEGMENTS];

    armSegments[0] = ArmSegment(146.0);
    armSegments[0].negativeConstraint = -3.14;
    armSegments[0].positiveConstraint = 0;

    armSegments[1] = ArmSegment(145.0);
    armSegments[1].negativeConstraint = -1.570796;
    armSegments[1].positiveConstraint = 1.570796;

    armSegments[2] = ArmSegment(155.0);
    armSegments[2].negativeConstraint = -DEFAULT_CONSTRAINT;
    armSegments[2].positiveConstraint = DEFAULT_CONSTRAINT;

    for (int i = 0; i < NUMBER_OF_ARM_SEGMENTS; ++i) {
        //Set damping for more fluent movements
        armSegments[i].damping = DEFAULT_DAMPING;

        // Register callback, used to iterate updates
        if (i > 0) armSegments[i - 1].registerChildArmSegment(&armSegments[i]);
    }

    Point ground = Point(0.0, 0.0, 0.0);
    armSegments[0].update(&ground, ARM_STARTING_ANGLE);

#ifdef VERBOSE
    std::cout << "\nStarting positions per armSegment: \n";
    for (int i = 0; i < NUMBER_OF_ARM_SEGMENTS; ++i) {
        armSegments[i].dump();
    }
#endif

    Ccd ccd = Ccd(armSegments, NUMBER_OF_ARM_SEGMENTS);
    success = ccd.computeCcdLink(pointToReach);

    // Apply Cyclic Coordinate Descent
    success = ccd.computeCcdLink(pointToReach);


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

    // Show whether the target was reached
    std::cout << "The target was reached: " << (success ? "True" : "False") << "\n";

    return 0;
}