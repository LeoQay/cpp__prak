#include <cmath>

class Vector
{
public:
    Vector(double x, double y, double z);
    double get_len() const;
    Vector & operator*= (double num);
private:
    double x, y, z;
};

Vector::Vector(double set_x, double set_y, double set_z)
: x(set_x), y(set_y), z(set_z) {}

double Vector::get_len() const
{
    return sqrt(x * x + y * y + z * z);
}

Vector & Vector::operator*=(double num)
{
    x *= num;
    y *= num;
    z *= num;
    return *this;
}