//
// Created by Geert Lens on 02/11/2018.
//

#include "arm.hpp"
#include "segment.hpp"
#include <Eigen/Dense>
#include <unistd.h>

#define MAX_IK_TRIES 100
#define IK_POS_THRESH 1.0
#define DAMPING true
#define DOF_RESTRICTED true

Arm::Arm() {

}

Arm::Arm(int segmentCount, int segmentLength) {
    this->segmentCount = segmentCount;

    segments[0] = segment(Eigen::Vector3d(0, 0, 0), segmentLength);

    for(int i = 1; i<segmentCount; ++i){
        segments[i] = segment(segments[i-1].getEnd(), segmentLength);
        segments[i].calculateEndPoint();
    }
}

/// Remap the angle between -PI and PI
double simplifyAngle(double angle)
{
    if( angle < -M_PI )
        angle += (2.0 * M_PI);
    else if( angle > M_PI )
        angle -= (2.0 * M_PI);
    return angle;
}

/// Calculate distance between two points
float vectorDistance(Eigen::Vector3d point1, Eigen::Vector3d point2){
    return sqrtf(powf(point1.x()-point2.x(), 2)+powf(point1.y()-point2.y(), 2)+powf(point1.z()-point2.z(), 2));
}

/// Print coordinates of all segments
void Arm::printCoords() {
    for(int i = 0; i<segmentCount; ++i){
        printf("segment %i: A:(%.2f, %.2f), B:(%.2f, %.2f)\n", i, segments[i].getBegin().x(),
                segments[i].getBegin().y(), segments[i].getEnd().x(), segments[i].getEnd().y());
    }
}

/// Draw arm segments
void Arm::drawArm(sf::RenderWindow *window, float WIDTH, float HEIGHT){
    for(int i = 0; i<segmentCount; ++i){
        segments[i].draw(window, WIDTH, HEIGHT);
    }
}

/// Update arm position
void Arm::updateIK(Eigen::Vector3d targetPos){
    Eigen::Vector3d rootPos,curEnd,desiredEnd,targetVector,curVector,crossResult;
    int link,tries;

    link = segmentCount-1;
    tries = 0;

    do{
        // POSITION OF ROOT OF THIS SEGMENT
        rootPos = segments[link].getBegin();

        // POSITION OF THE END EFFECTOR
        curEnd = segments[link].getEnd();

        // DESIRED END EFFECTOR POSITION
        desiredEnd = targetPos;

        if (vectorDistance(curEnd, desiredEnd) > IK_POS_THRESH){

            // Calculate vector between root and current end point
            curVector.x() = curEnd.x() - rootPos.x();
            curVector.y() = curEnd.y() - rootPos.y();
            curVector.z() = curEnd.z() - rootPos.z();

            // Calculate vector between root and target
            targetVector.x() = targetPos.x() - rootPos.x();
            targetVector.y() = targetPos.y() - rootPos.y();
            targetVector.z() = 0;

            // Normalize
            curVector.normalize();
            targetVector.normalize();

            double cosAngle = targetVector.dot(curVector);

            if(cosAngle < 0.99){
                // Get cross product to determine which way to rotate
                crossResult = targetVector.cross(curVector);
                double angleDeg = acos(cosAngle);
                if(crossResult.z() > 0.0){
                    angleDeg = simplifyAngle(angleDeg - segments[link].getAngle());
                    printf("Angle: %f\n", angleDeg);
                    segments[link].calculateEndPoint(rootPos, angleDeg);
                    segments[link].setBegin(segments[link - 1].getEnd());
                }
                else if (crossResult.z() < 0.0){
                    angleDeg = simplifyAngle(angleDeg + segments[link].getAngle());
                    printf("Angle: %f\n", angleDeg);
                    segments[link].calculateEndPoint(rootPos, angleDeg);
                    segments[link].setBegin(segments[link - 1].getEnd());
                }
            }
        }
        if (--link < 0) link = segmentCount - 1;
    }while(tries++ < MAX_IK_TRIES && vectorDistance(curEnd, desiredEnd) > IK_POS_THRESH);
}
