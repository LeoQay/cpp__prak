#ifndef CPP__PRAK_REGEX_CHECKER_H
#define CPP__PRAK_REGEX_CHECKER_H


#include <string>

#include "regex_parser.h"
#include "global_typedef.h"


class RegexChecker
{
public:
    RegexChecker(const std::string & re);

    bool check(const std::string & str);

    void exec_digit();
    void exec_char(char repr);
    void exec_empty();
    void exec_concat();
    void exec_or();
    void exec_iter_plus();
    void exec_iter_star();

private:
    RegexParser parser_;
    std::vector<grm_t> stack_;
    static const int digit_repr;
};


const int RegexChecker::digit_repr = 10000;


#endif //CPP__PRAK_REGEX_CHECKER_H
