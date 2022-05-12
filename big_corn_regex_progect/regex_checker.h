#ifndef CPP__PRAK_REGEX_CHECKER_H
#define CPP__PRAK_REGEX_CHECKER_H


#include <string>

#include "regex_parser.h"


class RegexChecker
{
public:
    RegexChecker(const std::string & re);

    bool check(const std::string & str);
private:
    RegexParser parser_;
};


#endif //CPP__PRAK_REGEX_CHECKER_H
