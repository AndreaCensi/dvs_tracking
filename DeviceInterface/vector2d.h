#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D
{
public:
    Vector2D(float xVal = 0, float yVal = 0);
    float toRadian(float degree);
    float toDegree(float radian);
    float norm();
    Vector2D normalize();
    float dot(Vector2D v);
    float normalizedDot(Vector2D v);
    Vector2D rotate(float angle);
    void reset();

    static const float PI;

    float x;
    float y;
};

#endif // VECTOR2D_H
