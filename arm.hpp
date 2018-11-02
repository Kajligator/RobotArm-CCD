//
// Created by Geert Lens on 02/11/2018.
//

#ifndef ROBOTARM_CCD_ARM_H
#define ROBOTARM_CCD_ARM_H

#include "segment.hpp"

class Arm {
    private:
        int segmentCount;
    public:
        segment segments[0];
        Arm();
        Arm(int segmentCount, int segmentLength);
        void printCoords();
        void drawArm(sf::RenderWindow *window, float WIDTH, float HEIGHT);
        void updateIK(Eigen::Vector3d targetPos);
};


#endif //ROBOTARM_CCD_ARM_H
