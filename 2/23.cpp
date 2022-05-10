/*
#include <cstdio>

class A
{
public:
    void f()
    {
        printf("f(%d)\n", y);
    }

    void f2()
    {
        printf("f2(%d)\n", y);
    }

    void f2(int x)
    {
        printf("f2(%d, %d)\n", y, x);
    }

    void f3()
    {
        printf("f3(%d)\n", y);
    }

    void f4()
    {
        printf("f4(%d)\n", y);
    }

    void f4(int x)
    {
        printf("f4(%d, %d)\n", y, x);
    }

    void set(int t)
    {
        y = t;
    }
private:
    int y;
};
 */

class B : public A
{
public:
    void f()
    {
        printf("B::f\n");
    }

    void f2()
    {
        printf("B::f2\n");
        A::f2();
    }


};

/*
int main()
{
    int x, y;
    scanf("%d%d", &x, &y);
    B b;
    A *p = &b;
    p->set(y);

    printf("1\n");
    b.f();

    printf("2\n");
    b.f2();

    printf("3\n");
    if (&B::f3 == &A::f3) {
        b.f3();
    }

    printf("4\n");
    b.f4();
    b.f4(x);

    printf("%d\n", y);
}
 */
