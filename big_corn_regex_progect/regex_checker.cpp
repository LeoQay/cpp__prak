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

    grm_ = stack_[0];
    stack_.pop_back();

    if (is_empty(grm_))
    {
        Symbol sym = Symbol(NOT_TERM, Symbol::next_name());
        grm_.start.insert(sym);
        sequence_t seq;
        seq.arr.emplace_back(TERM, 0);
        grm_.grm.emplace(sym, seq);
    }

    build_dfa();
}


bool RegexChecker::check(const std::string & str)
{
    std::string rev = str;
    std::reverse(rev.begin(), rev.end());

    return check_inside(Symbol(NOT_TERM, 0), rev);
}


bool RegexChecker::check_inside(Symbol cur_state, const std::string & str)
{
    if (str.empty() && grm_.start.find(cur_state) != grm_.start.end())
    {
        return true;
    }

    auto & paths = dfa_[cur_state];

    for (auto it : paths)
    {
        if (!str.empty() &&
        (it.first.repr == str[0] ||
        (it.first.repr == digit_repr && std::isdigit(str[0]))))
        {
            if (check_inside(it.second, str.substr(1)))
            {
                return true;
            }
        }
        else if (it.first.repr == 0)
        {
            if (check_inside(it.second, str.substr(0)))
            {
                return true;
            }
        }
    }

    return false;
}


void RegexChecker::exec_char(char repr)
{
    grm_t grm;
    Symbol sym = Symbol(NOT_TERM, Symbol::next_name());
    grm.start.insert(sym);
    sequence_t seq;
    seq.arr.emplace_back(TERM, repr);
    grm.grm.emplace(sym, seq);
    stack_.push_back(grm);
}


void RegexChecker::exec_digit()
{
    grm_t grm;
    Symbol sym = Symbol(NOT_TERM, Symbol::next_name());
    grm.start.insert(sym);
    sequence_t seq;
    seq.arr.emplace_back(TERM, digit_repr);
    grm.grm.emplace(sym, seq);
    stack_.push_back(grm);
}


void RegexChecker::exec_empty()
{
    stack_.emplace_back();
}


void RegexChecker::exec_or()
{
    if (stack_.size() < 2) throw std::exception();

    auto a = stack_[stack_.size() - 2], b = stack_[stack_.size() - 1];

    if (is_empty(a))
    {
        if (is_empty_chain(b))
        {
            stack_.push_back(b);
            return;
        }
    }

    if (is_empty(b))
    {
        if (is_empty_chain(a))
        {
            stack_.push_back(a);
            return;
        }
    }

    if (is_empty(a) && is_empty(b))
    {
        stack_.push_back(a);
        return;
    }

    grm_t res;

    res.start.insert(a.start.begin(), a.start.end());
    res.start.insert(b.start.begin(), b.start.end());

    res.grm.insert(a.grm.begin(), a.grm.end());
    res.grm.insert(b.grm.begin(), b.grm.end());

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

    if (is_empty(a))
    {
        stack_.push_back(b);
        return;
    }

    if (is_empty(b))
    {
        stack_.push_back(a);
        return;
    }

    res.start.insert(a.start.begin(), a.start.end());

    for (auto it = a.grm.begin(); it != a.grm.end(); )
    {
        if (it->second.size() == 1)
        {
            for (auto & finite : b.start)
            {
                sequence_t seq;
                seq.arr.emplace_back(finite);
                seq.arr.emplace_back(it->second.arr[0]);

                res.grm.emplace(it->first, seq);
            }

            it = a.grm.erase(it);
        }
        else
        {
            ++it;
        }
    }

    res.grm.insert(a.grm.begin(), a.grm.end());
    res.grm.insert(b.grm.begin(), b.grm.end());

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::exec_iter_plus()
{
    if (stack_.empty()) throw std::exception();

    grm_t a = stack_.back();
    stack_.pop_back();

    if (is_empty(a))
    {
        stack_.push_back(a);
        return;
    }

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

    if (is_empty(a))
    {
        stack_.push_back(a);
        return;
    }

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


void RegexChecker::build_dfa()
{

    for (auto & it : grm_.grm)
    {
        if (it.second.size() == 1)
        {
            dfa_[Symbol(NOT_TERM, 0)][it.second.arr[0]] = it.first;
        }
        else
        {
            dfa_[it.second.arr[0]][it.second.arr[1]] = it.first;
        }
    }
}


const int RegexChecker::digit_repr = 10000;


bool RegexChecker::is_empty(const grm_t & grm)
{
    return grm.grm.empty();
}


bool RegexChecker::is_empty_chain(const grm_t & grm)
{
    for (auto finite : grm.start)
    {
        auto range = grm.grm.equal_range(finite);

        for (auto it = range.first; it != range.second; it++)
        {
            if (it->second.size() == 1 && it->second.arr[0].is_empty())
            {
                return true;
            }
        }
    }
    return false;
}
