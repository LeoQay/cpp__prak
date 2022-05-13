#include <string>
#include <sstream>
#include <stdexcept>

#include "regex_checker.h"


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

    for (auto it : parser_)
    {
        switch (it.kind)
        {
            case PolizItem::Kind::CHAR:
                exec_char(it.repr);
                break;
            case PolizItem::Kind::DIGIT:
                exec_digit();
                break;
            case PolizItem::Kind::CONCAT:
                exec_concat();
                break;
            case PolizItem::Kind::OR:
                exec_or();
                break;
            case PolizItem::Kind::ITER_STAR:
                exec_iter_star();
                break;
            case PolizItem::Kind::ITER_PLUS:
                exec_iter_plus();
                break;
            case PolizItem::Kind::EMPTY:
                exec_empty();
                break;
            default:
                break;
        }
    }
}


bool RegexChecker::check(const std::string & str)
{
}


void RegexChecker::exec_char(char repr)
{
    grm_t grm;
    sequence_t seq;
    seq.arr.emplace_back(TERM, repr);
    grm.grm.emplace(Symbol(NOT_TERM, Symbol::next_name()), seq);
    stack_.push_back(grm);
}


void RegexChecker::exec_digit()
{
    grm_t grm;
    sequence_t seq;
    seq.arr.emplace_back(TERM, digit_repr);
    grm.grm.emplace(Symbol(NOT_TERM, Symbol::next_name()), seq);
    stack_.push_back(grm);
}


void RegexChecker::exec_empty()
{
    grm_t grm;
    sequence_t seq;
    seq.arr.emplace_back(TERM, 0);
    grm.grm.emplace(Symbol(NOT_TERM, Symbol::next_name()), seq);
    stack_.push_back(grm);
}


void RegexChecker::exec_concat()
{

}
