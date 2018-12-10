#include <cmath>
#include <iostream>

#include "Ccd.h"
#include "Vector3.h"

#define VERBOSE

Ccd::Ccd(ArmSegment *armSegments, const unsigned int AMOUNT_OF_LINKS) : armSegments(armSegments),
                                                                        AMOUNT_OF_LINKS(AMOUNT_OF_LINKS) {}
/// Calculate distance between vector a and vector b
double calculateDistanceSquared(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;

    return dx * dx + dy * dy + dz * dz;
}

/// Apply damping to specific segment.
void applyDamping(double *angle, ArmSegment *armSegment) {
    if (*angle > armSegment->damping) {
        *angle = armSegment->damping;
    }
}

double vectorDotProductAngle(Vector3 *current, Vector3 *target) {
    Point a = current->end;
    Point b = target->end;

    double dotProductScalar = a.x * b.x + a.z * b.z;
    return acos(dotProductScalar);
}

Vector3 vectorCrossProduct(Vector3 *current, Vector3 *target) {
    Point crossProduct;
    Point a = current->end;
    Point b = target->end;

    crossProduct.z = a.x * b.y - a.y * b.x;
    crossProduct.y = a.z * b.x - a.x * b.z;
    crossProduct.x = a.y * b.z - a.z * b.y;

    return Vector3(crossProduct);
}

double calculateArmSegmentAngle(ArmSegment *currentArmSegment, ArmSegment *finalArmSegment, Point *targetPoint) {
    Vector3 currentVector(currentArmSegment->origin, finalArmSegment->end);
    Vector3 targetVector(currentArmSegment->origin, *targetPoint);

    //Normalize the vectors to make the dot product calculation easier.
    currentVector.normalize();
    targetVector.normalize();

    //Dot product shows angle, cross shows direction
    double angleInRadians = vectorDotProductAngle(&currentVector, &targetVector);
    Vector3 direction = vectorCrossProduct(&currentVector, &targetVector);

    //Apply damping before picking direction, the just calculated angle is always positive.
    applyDamping(&angleInRadians, currentArmSegment);

    //The direction follows from the crossProduct. A positive translation means go left.
    if (direction.end.y > 0) {
        angleInRadians = 0 - angleInRadians;
    }

    //The preferred angle
    return angleInRadians;
}

bool Ccd::computeCcdLink(Point targetPoint) {
    //We start with the last armSegment, and work our way down. Then repeat.
    int activeArmSegmentCounter = AMOUNT_OF_LINKS - 1;

    double distance;
    ArmSegment *currentArmSegment = &armSegments[activeArmSegmentCounter];
    ArmSegment *finalArmSegment = currentArmSegment; //Contains arm end-point

    int tries = 0; //Sometimes the target cannot be reached
    while (tries++ < AMOUNT_OF_TRIES) {
        //Check whether we are close enough to the target point
        distance = calculateDistanceSquared(targetPoint, finalArmSegment->end);

#ifdef VERBOSE
                std::cout << "[CCD] Current squared distance: " << distance << "\n";
#endif

        //Check if we are done
        if (distance < TARGET_REACHED_THRESHOLD) return true;

        //Calculate a new angle for an armSegment
        double angleInRadians = calculateArmSegmentAngle(currentArmSegment, finalArmSegment, &targetPoint);

        //Update the arm, this is an iterative process
        currentArmSegment->update(angleInRadians);

        //Switch to next armSegment for the next try.
        if (--activeArmSegmentCounter < 0) activeArmSegmentCounter = AMOUNT_OF_LINKS - 1;
        currentArmSegment = &armSegments[activeArmSegmentCounter];
    }

    //The armSegments could not reach the target point
    return false;
}