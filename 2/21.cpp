/*
#include <cstdio>

class A
{
public:
    A(int x = 0) : t(x)
    {
        printf("A(%d)\n", x);
    }

    A(const A &a) : t(a.t)
    {
        printf("A(A&)\n");
    }

    ~A()
    {
        printf("~A(%d)\n", t);
    }
private:
    int t;
};
*/


class B : public A
{
public:
    B(int x) : A(2 * x), var(10 * x)
    {
        printf("B(%d)\n", x);
    }

    B(const B & obj) : A(obj), var()
    {
        printf("B(B&)\n");
        ost = new A(1);
    }

    B & operator= (const B & obj) = delete;

    ~B()
    {
        printf("~B\n");
        delete ost;
    }

private:
    A var;
    A * ost = nullptr;
};


/*
int main()
{
    int x, y; scanf("%d%d", &x, &y);
    B b(x);
    A *p = &b;
    if (p) {
        B b2(y);
        B b3(b);
    }
}
*/
