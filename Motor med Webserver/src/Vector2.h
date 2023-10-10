#include <global.h>

class VectorZWEI
{
private:
public:
    double x;
    double y;

    VectorZWEI(double x = 0, double y = 0)
    {
        this->x = x;
        this->y = y;
    };
    void fromPolar(double radians, double length);

    double angle(VectorZWEI *vector);
    double dot(VectorZWEI *vector);
    double magnitude();

    VectorZWEI operator*(VectorZWEI &v1);
    VectorZWEI operator+(VectorZWEI &v1);
    VectorZWEI operator*(int32_t &num);
    VectorZWEI operator/(int32_t &num);
};