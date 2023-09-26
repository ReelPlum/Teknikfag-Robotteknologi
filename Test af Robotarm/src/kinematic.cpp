#include <kinematic.h>

Vector2 getPosition(double angle1, double angle2, int32_t l1, int32_t l2){
    Vector2 v1;
    v1.fromPolar(angle1, l1);

    Vector2 v2;
    v2.fromPolar(angle2, l2);

    return v1 + v2;
}