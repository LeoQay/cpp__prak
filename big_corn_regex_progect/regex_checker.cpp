#include <string>
#include <sstream>
#include <stdexcept>

#include "regex_checker.h"
#include "left_grm_to_dfa.h"
#include "delete_bad.h"
#include "delete_empty_rules.h"


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


void RegexChecker::exec_or()
{
    if (stack_.size() < 2) throw std::exception();

    auto a = stack_.size() - 2, b = stack_.size() - 1;

    grm_t res;

    res.start.insert(stack_[a].start.begin(), stack_[a].start.end());
    res.start.insert(stack_[b].start.begin(), stack_[b].start.end());

    res.grm.insert(stack_[a].grm.begin(), stack_[a].grm.end());
    res.grm.insert(stack_[b].grm.begin(), stack_[b].grm.end());

    stack_.pop_back();
    stack_.pop_back();

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::exec_concat()
{
    if (stack_.size() < 2) throw std::exception();

    grm_t b = stack_.back();
    stack_.pop_back();
    grm_t a = stack_.back();
    stack_.pop_back();

    grm_t res;

    res.start.insert(a.start.begin(), a.start.end());

    res.grm.insert(a.grm.begin(), a.grm.end());
    res.grm.insert(b.grm.begin(), b.grm.end());

    for (auto & it : a.grm)
    {
        if (it.second.size() == 1)
        {
            for (auto & finite : b.start)
            {
                sequence_t seq;
                seq.arr.emplace_back(finite);
                seq.arr.emplace_back(it.second.arr[0]);

                res.grm.emplace(it.first, seq);
            }
        }
    }

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::exec_iter_plus()
{
    if (stack_.empty()) throw std::exception();

    grm_t a = stack_.back();
    stack_.pop_back();

    grm_t res = a;

    for (auto & it : a.grm)
    {
        if (it.second.size() == 1)
        {
            for (auto & finite : a.start)
            {
                sequence_t seq;
                seq.arr.emplace_back(finite);
                seq.arr.emplace_back(it.second.arr[0]);

                res.grm.emplace(it.first, seq);
            }
        }
    }

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::exec_iter_star()
{
    if (stack_.empty()) throw std::exception();

    grm_t a = stack_.back();
    stack_.pop_back();

    grm_t res = a;

    for (auto & it : a.grm)
    {
        if (it.second.size() == 1)
        {
            for (auto & finite : a.start)
            {
                sequence_t seq;
                seq.arr.emplace_back(finite);
                seq.arr.emplace_back(it.second.arr[0]);

                res.grm.emplace(it.first, seq);
            }
        }
    }

    sequence_t seq;
    seq.arr.emplace_back(TERM, 0);

    for (auto & it : res.start)
    {
        res.grm.emplace(it, seq);
    }

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::process_grm(grm_t & grm)
{
    LeftDFAMgr mgr;
    mgr.build_dfa(grm);
    grm = mgr.result;

    DeleteEmptyRules del;
    del.run(grm);

    ProductionContextFreeGrammar prod;
    prod.run(grm);
}
