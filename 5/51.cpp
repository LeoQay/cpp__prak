#include <iostream>

template<typename T> void f()
{
    std::cout << "Generic" << std::endl;
}

template<> void f<int> ()
{
    std::cout << "Int" << std::endl;
}
