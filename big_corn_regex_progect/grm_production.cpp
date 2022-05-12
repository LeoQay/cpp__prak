#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <vector>

#include "grm_production.h"



class GrammarTypeChecker
{
public:
    static bool check_sym_not_terminal(char sym);
};

bool GrammarTypeChecker::check_sym_not_terminal(char sym)
{
    return sym >= 'A' && sym <= 'Z';
}


void NoChildContextFreeGrammar::run(grm_t & grm)
{
    clear();
    build_N(grm);
    collect_non_terminal(grm);
    make_bad_set();
    do_delete(grm);
}

void NoChildContextFreeGrammar::build_N(const grm_t & grm)
{
    while (do_step(grm));
}

bool NoChildContextFreeGrammar::do_step(const grm_t & grm)
{
    for (auto & rule : grm)
    {
        if (is_has_child_str(rule.second))
        {
            cur_N.insert(rule.first[0]);
        }
    }

    if (cur_N.size() == prev_N.size())
    {
        return false;
    }

    prev_N.insert(cur_N.begin(), cur_N.end());
    cur_N.clear();

    return true;
}

bool NoChildContextFreeGrammar::is_has_child_str(const std::string & str)
{
    return std::all_of(str.begin(), str.end(),
                       [this] (char sym) { return is_has_child(sym); } );
}

bool NoChildContextFreeGrammar::is_has_child(char non_terminal)
{
    return !GrammarTypeChecker::check_sym_not_terminal(non_terminal) ||
           prev_N.find(non_terminal) != prev_N.end();
}

void NoChildContextFreeGrammar::collect_non_terminal(const grm_t & grm)
{
    for (auto & rule : grm)
    {
        collect_str_non_terminal(rule.first);
        collect_str_non_terminal(rule.second);
    }
}

void
NoChildContextFreeGrammar::collect_str_non_terminal(const std::string & str)
{
    for (char sym : str)
    {
        if (GrammarTypeChecker::check_sym_not_terminal(sym))
        {
            non_terminals.insert(sym);
        }
    }
}

void NoChildContextFreeGrammar::make_bad_set()
{
    std::set_difference(non_terminals.begin(), non_terminals.end(),
                        prev_N.begin(), prev_N.end(),
                        std::inserter(bad_set, bad_set.begin()));
}

void NoChildContextFreeGrammar::clear()
{
    prev_N.clear();
    cur_N.clear();
    non_terminals.clear();
    bad_set.clear();
}

void NoChildContextFreeGrammar::do_delete(grm_t & grm)
{
    for (char sym : bad_set)
    {
        do_delete_sym(grm, sym);
    }
}

void NoChildContextFreeGrammar::do_delete_sym(grm_t & grm, char sym)
{
    for (auto it = grm.begin(); it != grm.end();)
    {
        auto cur = it;
        it++;

        if (static_cast<int>(cur->first.find(sym)) != -1 ||
            static_cast<int>(cur->second.find(sym)) != -1)
        {
            grm.erase(cur);
        }
    }
}


void NoAvailableContextFreeGrammar::run(grm_t & grm)
{
    clear();
    make_good_set(grm);
    collect_non_terminal(grm);
    make_bad_set();
    do_delete(grm);
}

void NoAvailableContextFreeGrammar::make_good_set(const grm_t & grm)
{
    cur_set.insert(start);

    while (!cur_set.empty())
    {
        good_set.insert(cur_set.begin(), cur_set.end());

        std::string s = "_";
        for (char sym : cur_set)
        {
            s[0] = sym;
            do_step(grm, s);
        }

        cur_set = temp_set;
        temp_set.clear();
    }
}

void
NoAvailableContextFreeGrammar::do_step(const grm_t & grm, std::string & str)
{
    auto range = grm.equal_range(str);

    for (auto it = range.first; it != range.second; it++)
    {
        for (char sym : it->second)
        {
            if (GrammarTypeChecker::check_sym_not_terminal(sym) &&
                good_set.find(sym) == good_set.end())
            {
                temp_set.insert(sym);
            }
        }
    }
}

void NoAvailableContextFreeGrammar::collect_non_terminal(const grm_t & grm)
{
    for (auto & rule : grm)
    {
        collect_str_non_terminal(rule.first);
        collect_str_non_terminal(rule.second);
    }
}

void
NoAvailableContextFreeGrammar::collect_str_non_terminal(const std::string & str)
{
    for (char sym : str)
    {
        if (GrammarTypeChecker::check_sym_not_terminal(sym))
        {
            non_terminals.insert(sym);
        }
    }
}

void NoAvailableContextFreeGrammar::clear()
{
    bad_set.clear();
    good_set.clear();
    temp_set.clear();
    cur_set.clear();
    non_terminals.clear();
}

void NoAvailableContextFreeGrammar::make_bad_set()
{
    std::set_difference(non_terminals.begin(), non_terminals.end(),
                        good_set.begin(), good_set.end(),
                        std::inserter(bad_set, bad_set.begin()));
}

void NoAvailableContextFreeGrammar::do_delete(grm_t & grm)
{
    for (char sym : bad_set)
    {
        do_delete_sym(grm, sym);
    }
}

void NoAvailableContextFreeGrammar::do_delete_sym(grm_t & grm, char sym)
{
    for (auto it = grm.begin(); it != grm.end();)
    {
        auto cur = it;
        it++;

        if (static_cast<int>(cur->first.find(sym)) != -1 ||
            static_cast<int>(cur->second.find(sym)) != -1)
        {
            grm.erase(cur);
        }
    }
}
