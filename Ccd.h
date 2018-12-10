//
// Created by Geert Lens on 02/11/2018.
//

#ifndef ROBOTARM_CCD_ARM_H
#define ROBOTARM_CCD_ARM_H

#include "Point.h"
#include "ArmSegment.h"

#define AMOUNT_OF_TRIES 100
#define TARGET_REACHED_THRESHOLD 0.001

class Ccd {
    private:
        ArmSegment *armSegments;
        const unsigned int AMOUNT_OF_LINKS;
    public:
        Ccd(ArmSegment *armSegments, unsigned int AMOUNT_OF_LINKS);

        bool computeCcdLink(Point targetPoint);
};


#endif //ROBOTARM_CCD_ARM_H
