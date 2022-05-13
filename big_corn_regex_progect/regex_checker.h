#ifndef CPP__PRAK_REGEX_CHECKER_H
#define CPP__PRAK_REGEX_CHECKER_H


#include <string>

#include "regex_parser.h"
#include "global_typedef.h"


using usefull_dfa_t = std::map<Symbol, std::map<Symbol, Symbol>>;


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
    bool check_inside(Symbol cur_state, const std::string & str);

    static void process_grm(grm_t & grm);

    void build_dfa();

    static bool is_empty(const grm_t & grm);
    static bool is_empty_chain(const grm_t & grm);

    grm_t grm_;
    usefull_dfa_t dfa_;

    RegexParser parser_;
    std::vector<grm_t> stack_;
    static const int digit_repr;
};



#endif //CPP__PRAK_REGEX_CHECKER_H
