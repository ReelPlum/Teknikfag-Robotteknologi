#include <Vector2.h>
#include <math.h>

void Vector2::fromPolar(double angle, double length)
{
    this->x = length * cos(angle);
    this->y = length * sin(angle);
};

double Vector2::dot(Vector2 *vector){
    return this->x*vector->x + this->y*vector->y;
};
double Vector2::magnitude(){
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
};
double Vector2::angle(Vector2 *vector){
    return this->dot(vector)/(this->magnitude()*vector->magnitude());
};

Vector2 Vector2::operator*(Vector2 &v1)
{
    Vector2 multiplied(v1.x * this->x, v1.y * this->y);
    return multiplied;
};
Vector2 Vector2::operator+(Vector2 &v1)
{
    Vector2 added(v1.x + this->x, v1.y + this->y);
    return added;
};
Vector2 Vector2::operator*(int32_t &num)
{
    Vector2 multiplied(this->x * num, this->y * num);
    return multiplied;
};
Vector2 Vector2::operator/(int32_t &num)
{
    Vector2 divided(this->x / num, this->y / num);
    return divided;
}