#include <iostream>
#include <fstream>
#include <stdexcept>

#include "regex_parser.h"


int main()
{
    std::ifstream fin("file.txt");

    RegexParser parser;

    try {
        parser.parse(fin);
    }
    catch (std::exception &)
    {
        std::cout << "NO" << std::endl;
        return 0;
    }

    parser.print(std::cout, "\n");
    return 0;
}
