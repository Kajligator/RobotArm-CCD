#ifndef ROBOTARM_CCD_VECTOR2D_H
#define ROBOTARM_CCD_VECTOR2D_H


#include <math.h>

class Vector2
{
    public:
        Vector2();
        Vector2(float x, float y);
        float x;
        float y;
        void set(float x, float y);
        Vector2 * div(float n);
        Vector2 * add(Vector2& anotherVector);
        Vector2 * sub(Vector2& anotherVector);
        Vector2 * mult(float n);
        Vector2 * normalize();
        float dot(Vector2& anotherVector);
        float magnitude();
        Vector2 * setMag(float len);
        float mag();
        float angle();
};


#endif //ROBOTARM_CCD_VECTOR2D_H
