#include <Vector2.h>
#include <math.h>

void VectorZWEI::fromPolar(double angle, double length)
{
    this->x = length * cos(angle);
    this->y = length * sin(angle);
};

double VectorZWEI::dot(VectorZWEI *vector){
    return this->x*vector->x + this->y*vector->y;
};
double VectorZWEI::magnitude(){
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
};
double VectorZWEI::angle(VectorZWEI *vector){
    return this->dot(vector)/(this->magnitude()*vector->magnitude());
};

VectorZWEI VectorZWEI::operator*(VectorZWEI &v1)
{
    VectorZWEI multiplied(v1.x * this->x, v1.y * this->y);
    return multiplied;
};
VectorZWEI VectorZWEI::operator+(VectorZWEI &v1)
{
    VectorZWEI added(v1.x + this->x, v1.y + this->y);
    return added;
};
VectorZWEI VectorZWEI::operator*(int32_t &num)
{
    VectorZWEI multiplied(this->x * num, this->y * num);
    return multiplied;
};
VectorZWEI VectorZWEI::operator/(int32_t &num)
{
    VectorZWEI divided(this->x / num, this->y / num);
    return divided;
}