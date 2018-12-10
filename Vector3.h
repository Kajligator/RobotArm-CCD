//
// Created by Geert Lens on 10/12/2018.
//

#ifndef ROBOTARM_CCD_VECTOR3_H
#define ROBOTARM_CCD_VECTOR3_H


#include "Point.h"

class Vector3 {
    private:
        Point origin;
    public:
        Point end;

        explicit Vector3(Point end);

        Vector3(Point origin, Point end);

        void normalize();

        void dump();
};


#endif //ROBOTARM_CCD_VECTOR3_H
