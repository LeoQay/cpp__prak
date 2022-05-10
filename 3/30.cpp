/*
A::f
A::g
B::f
B::g
C::f
B::g
A::h
число, введенное пользователем
 */

/// MISSED CODE. PUT YOUR CODE HERE.
///////////////////////////////////////////////

#include <iostream>

int x;

class A
{
public:
    virtual void f();
    virtual void g();
    void h();
    virtual ~A();
};

void A::h()
{
    std::cout << "A::h" << std::endl;
}

A::~A() {}

class B : public A
{
public:
    void f();
    void g();
    ~B();
};

B::~B() {}

class C : public B
{
public:
    C();
    C(const C &) = delete;
    C & operator= (const C &) = delete;
    ~C();
    void f();
    void h();
private:
    A * val;
};

////////////////////////////////////////////

/*

void A::f() {
    std::cout << "A::f" << std::endl;
}

void A::g() {
    std::cout << "A::g" << std::endl;
}

void B::f() {
    std::cout << "B::f" << std::endl;
}

void B::g() {
    std::cout << "B::g" << std::endl;
}

C::C(): val(new A) {
}

C::~C() {
    delete val;
}

void C::f() {
    std::cout << "C::f" << std::endl;
}

void C::h() {
    std::cout << "C::h" << std::endl;
}


int main() {
    std::cin >> x;

    A * a = new A();
    A * b = new B();
    A * c = new C();

    a->f(); // A::f
    a->g(); // A::g

    b->f(); // B::f
    b->g(); // B::g

    c->f(); // C::f
    c->g(); // B::g
    c->h(); // A::h

    delete a;
    delete b;
    delete c;

    std::cout << x << std::endl;
    return 0;
}

*/
