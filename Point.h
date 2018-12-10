//
// Created by Geert Lens on 10/12/2018.
//

#ifndef ROBOTARM_CCD_POINT_H
#define ROBOTARM_CCD_POINT_H

class Point {
    public:
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

        Point();

        Point(double x, double y, double z);

        void dump();
};


#endif //ROBOTARM_CCD_POINT_H
