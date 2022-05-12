#include <string>
#include <sstream>
#include <stdexcept>

#include "regex_checker.h"
#include "left_grm_to_dfa.h"


RegexChecker::RegexChecker(const std::string & re) : parser_()
{
    std::stringstream stream;
    stream << re;

    try {
        parser_.parse(stream);
    }
    catch (...)
    {
        throw std::runtime_error("INCORRECT REGEX");
    }

    parser_.print(std::cout, "\n");
}


bool RegexChecker::check(const std::string & str)
{

}
