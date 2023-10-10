#include <global.h>

class Vector2
{
private:
public:
    double x;
    double y;

    Vector2(double x = 0, double y = 0)
    {
        this->x = x;
        this->y = y;
    };
    void fromPolar(double radians, double length);

    double angle(Vector2 *vector);
    double dot(Vector2 *vector);
    double magnitude();

    Vector2 operator*(Vector2 &v1);
    Vector2 operator+(Vector2 &v1);
    Vector2 operator*(int32_t &num);
    Vector2 operator/(int32_t &num);
};