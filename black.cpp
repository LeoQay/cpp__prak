#include <iostream>


class Cool
{
public:
    explicit Cool(int x = 0) : x(x) {}

    Cool operator - () const
    {
        std::cout << "Unary minus for " << x << std::endl;
        return Cool(-x);
    }

    Cool operator+ (const Cool & obj) const
    {
        std::cout << "Add for " << x << " and " << obj.x << std::endl;
        return Cool(x + obj.x);
    }

    Cool operator- (const Cool & obj) const
    {
        std::cout << "Sub for " << x << " and " << obj.x << std::endl;
        return Cool(x - obj.x);
    }

    Cool operator* (const Cool & obj) const
    {
        std::cout << "Mul for " << x << " and " << obj.x << std::endl;
        return Cool(x * obj.x);
    }

    Cool operator/ (const Cool & obj) const
    {
        std::cout << "Div for " << x << " and " << obj.x << std::endl;
        return Cool(x / obj.x);
    }

    int x;
};


int main()
{
    int x;

    Cool a(2), b(10), c(14), d(3);
    Cool res = - (a + b) / (c - d);

    return 0;
}