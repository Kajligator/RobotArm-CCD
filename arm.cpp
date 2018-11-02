//
// Created by Geert Lens on 02/11/2018.
//

#include "arm.hpp"
#include "segment.hpp"
#include <Eigen/Dense>

#define MAX_IK_TRIES 100
#define IK_POS_THRESH 1.0

Arm::Arm() {

}

Arm::Arm(int segmentCount, int segmentLength) {
    this->segmentCount = segmentCount;

    segments[0] = segment(Eigen::Vector3d(0, 0, 0), segmentLength);

    for(int i = 1; i<segmentCount; i++){
        segments[i] = segment(segments[i-1].getEnd(), segmentLength);
        segments[i].calculateEndPoint();
    }
}

void Arm::printCoords() {
    for(int i = 0; i<segmentCount; i++){
        printf("segment %i: A:(%.2f, %.2f), B:(%.2f, %.2f)\n", i, segments[i].getBegin().x(),
                segments[i].getBegin().y(), segments[i].getEnd().x(), segments[i].getEnd().y());
    }
}

void Arm::drawArm(sf::RenderWindow *window, float WIDTH, float HEIGHT){
    for(int i = 0; i<segmentCount; i++){
        segments[i].draw(window, WIDTH, HEIGHT);
    }
}

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
            curVector.x() = curEnd.x() - rootPos.x();
            curVector.y() = curEnd.y() - rootPos.y();
            curVector.z() = curEnd.z() - rootPos.z();

            targetVector.x() = targetPos.x() - rootPos.x();
            targetVector.y() = targetPos.y() - rootPos.y();
            targetVector.z() = 0;

            curVector.normalize();
            targetVector.normalize();

            double cosAngle = targetVector.dot(curVector);

            if(cosAngle < 0.99){
                crossResult = targetVector.cross(curVector);
                double angleDeg = acos(cosAngle);
                if(crossResult.z() > 0.0){
                    angleDeg = angleDeg - segments[link].getAngle();  // GET THE ANGLE
                    printf("Angle: %f\n", angleDeg);
                    segments[link].calculateEndPoint(rootPos, angleDeg);
                }
                else if (crossResult.z() < 0.0){
                    angleDeg = angleDeg - segments[link].getAngle();
                    printf("Angle: %f\n", angleDeg);
                    segments[link].calculateEndPoint(rootPos, angleDeg);
                }
            }
        }
        if (--link < 0) link = segmentCount - 1; // START OF THE CHAIN, RESTART

    }while(tries++ < MAX_IK_TRIES && vectorDistance(curEnd, desiredEnd) > IK_POS_THRESH);

    if(vectorDistance(curEnd, desiredEnd) > IK_POS_THRESH){
        printf("Reached point\n");
    }

}

float Arm::vectorDistance(Eigen::Vector3d point1, Eigen::Vector3d point2){
    return sqrtf(powf(point1.x()-point2.x(), 2)+powf(point1.y()-point2.y(), 2));
}
