#include <iostream>

class Complex
{
public:
    Complex(double re, double im);
    void add(const Complex & obj);
    void add(double re, double im);
    void print() const;
private:
    double real, image;
};

Complex::Complex(double re, double im) : real(re), image(im) {}

void Complex::add(const Complex & obj)
{
    real += obj.real;
    image += obj.image;
}

void Complex::add(double re, double im)
{
    real += re;
    image += im;
}

void Complex::print() const
{
    std::cout << real << " " << image;
}
