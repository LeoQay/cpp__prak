#include <vector>

template<typename IteratorT, typename ElemT>
bool check(IteratorT begin, IteratorT end, ElemT open_b, ElemT close_b)
{
    std::vector<ElemT> stack;

    for (IteratorT it = begin; it != end; it++)
    {
        if (*it == open_b)
        {
            stack.push_back(open_b);
        }
        else if (*it == close_b)
        {
            if (stack.empty() || stack.back() != open_b)
            {
                return false;
            }
            stack.pop_back();
        }
    }

    return stack.empty();
}

#include <iostream>
#include <string>

int main()
{
    std::string str;

    getline(std::cin, str);

    if (str.empty())
    {
        return 0;
    }

    if (check(str.begin(), str.end(), '(', ')'))
    {
        std::cout << "Correct" << std::endl;
    }
    else
    {
        std::cout << "Incorrect" << std::endl;
    }

    return 0;
}
