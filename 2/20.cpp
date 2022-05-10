/*

#include <iostream>

class A
{
public:
    A(int a = 0) : x(a + 1) {}

    int get() const
    {
        return x;
    }
private:
    int x;
};
*/

class B : public A
{
public:
    B(int a) : A(a + 1) {}
    B(const B & obj) : A(obj.get()) {}
};


/*
int main()
{
    int x, y;
    std::cin >> x >> y;
    B b(x);
    A *p = &b;
    B b2 = b;
    A *p2 = &b2;
    std::cout << p->get() << " " << p2->get() << " " << y << std::endl;
    return 0;
}
*/
