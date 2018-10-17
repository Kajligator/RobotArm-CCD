#ifndef ROBOTARM_CCD_SEGMENT_H
#define ROBOTARM_CCD_SEGMENT_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "Eigen/Core"

class segment {
    private:
        Eigen::Vector3d begin;
        Eigen::Vector3d end;
        double length;
        double angle;

    public:
        segment();
        segment(Eigen::Vector3d end, double length);
        void calculateEndPoint(Eigen::Vector3d bp, double length, double angle);
        void calculateEndPoint();
        void draw(sf::RenderWindow *window, float screen_width, float screen_height);
        Eigen::Vector3d getBegin();
        Eigen::Vector3d getEnd();
        void setEnd(Eigen::Vector3d);
        double getLength();
        double getAngle();
        void setAngle(double angle);
};


#endif //ROBOTARM_CCD_SEGMENT_H
