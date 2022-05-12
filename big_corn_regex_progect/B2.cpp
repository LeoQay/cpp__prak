#include <iostream>
#include <string>

#include "regex_checker.h"


int main()
{
    std::string current;
    if (!getline(std::cin, current))
    {
        return 0;
    }

    try
    {
        RegexChecker checker(current);

        while (getline(std::cin, current))
        {
            std::cout << (checker.check(current) ? "YES" : "NO") << std::endl;
        }
    }
    catch (std::exception & err)
    {
        std::cout << err.what() << std::endl;
    }

    return 0;
}
