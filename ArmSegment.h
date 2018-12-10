#ifndef ROBOTARM_CCD_SEGMENT_H
#define ROBOTARM_CCD_SEGMENT_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "Point.h"

class ArmSegment {
    private:
        double length;
        double angle_ground = 0.0;
        double angle_joint = 0.0;

        ArmSegment *childArmSegment = nullptr;

        void calculateNewEndPoint();

    public:
        double damping = 6.28319;
        double negativeConstraint = -6.28319;
        double positiveConstraint = 6.28319;
        Point origin;
        Point end;

        ArmSegment();

        explicit ArmSegment(double length);

        ArmSegment(double length, double angle);
        ArmSegment(const Point &origin, double length);
        ArmSegment(const Point &origin, double length, double angle);

        void registerChildArmSegment(ArmSegment *armSegment);

        void update(double angleTranslation);
        void update(Point *newOrigin, double angleTranslation);

        void draw(sf::RenderWindow *window);

        void dump();
};


#endif //ROBOTARM_CCD_SEGMENT_H
