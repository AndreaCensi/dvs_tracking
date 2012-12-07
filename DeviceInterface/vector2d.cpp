#include "vector2d.h"
#include "math.h"

const float Vector2D::PI = 3.14159265f;

Vector2D::Vector2D(float xVal, float yVal)
{
    x = xVal;
    y = yVal;
}

float Vector2D::toRadian(float degree){
    float result = degree*PI/180.0f;
    return result;
}

float Vector2D::toDegree(float radian){
    float result = radian*180.0f/PI;
    return result;
}

float Vector2D::norm()
{
    return sqrt( x*x + y*y);
}

Vector2D Vector2D::normalize(){
    float l = norm();
    Vector2D v;
    v.x = x/l;
    v.y = y/l;
    return v;
}

float Vector2D::dot( Vector2D v )
{
    return ( x*v.x + y*v.y );
}

float Vector2D::normalizedDot(Vector2D v){
    Vector2D normedV1 = normalize();
    Vector2D normedV2 = v.normalize();
    return ( normedV1.x*normedV2.x + normedV1.y*normedV2.y );
}

Vector2D Vector2D::rotate( float angle ){
    Vector2D v;
    v.x = cos(angle)*x -  sin(angle)*y;
    v.y = sin(angle)*x + cos(angle)*y;
    return v;
}

void Vector2D::reset(){
    x = 0.0f;
    y = 0.0f;
}
