#include <iostream>

template<class T>
void g(const T & container)
{
    auto start = container.begin();
    auto it = container.end();

    for (int i = 0; i < 3; i++)
    {
        if (start == it)
        {
            return;
        }
        it--;
    }

    it = container.end();
    it--;
    auto sum = *it;
    it--;
    sum = sum + (*it);
    it--;
    sum = sum + (*it);

    std::cout << sum;
    std::cout << std::endl;
}

/*
#include <list>
#include <vector>

int main()
{
    std::list<int> l;
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);

    g(l);
}
*/
