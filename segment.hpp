#ifndef ROBOTARM_CCD_SEGMENT_H
#define ROBOTARM_CCD_SEGMENT_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "vector2.hpp"

class Segment {
    private:
    float len;
    float angle;
    Vector2 a;
    Vector2 b;

    public:
        Segment();
        Segment(float x, float y, float len, float angle);
        Segment(Segment& parent, float len, float angle);
        void calculateB();
        void update();
        void draw(sf::RenderWindow *window);
        Vector2 getA();
        void setA(Vector2 *pos);
        void setB(Vector2 *pos);
        Vector2 getB();
        Segment *parent;
        Segment *child;
};


#endif //ROBOTARM_CCD_SEGMENT_H
