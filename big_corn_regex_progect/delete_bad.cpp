#include <algorithm>

#include "delete_bad.h"
#include "global_typedef.h"


void NoChildContextFreeGrammar::run(grm_t & grm)
{
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
    for (auto & rule : grm.grm)
    {
        if (is_has_child_str(rule.second))
        {
            cur_N.insert(rule.first);
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

bool NoChildContextFreeGrammar::is_has_child_str(const sequence_t & chain)
{
    return std::all_of(chain.arr.begin(), chain.arr.end(),
                       [this] (Symbol sym) { return is_has_child(sym); } );
}

bool NoChildContextFreeGrammar::is_has_child(Symbol non_terminal)
{
    return non_terminal.kind == TERM || prev_N.find(non_terminal) != prev_N.end();
}

void NoChildContextFreeGrammar::collect_non_terminal(const grm_t & grm)
{
    for (auto & rule : grm.grm)
    {
        non_terminals.insert(rule.first);
        collect_str_non_terminal(rule.second);
    }
}

void
NoChildContextFreeGrammar::collect_str_non_terminal(const sequence_t & str)
{
    for (Symbol sym : str.arr)
    {
        if (sym.kind == NOT_TERM)
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

void NoChildContextFreeGrammar::do_delete(grm_t & grm)
{
    for (Symbol sym : bad_set)
    {
        do_delete_sym(grm, sym);
    }
}

void NoChildContextFreeGrammar::do_delete_sym(grm_t & grm, Symbol sym)
{
    for (auto it = grm.grm.begin(); it != grm.grm.end();)
    {
        auto cur = it;
        it++;

        if (cur->first == sym ||
        std::find(cur->second.arr.begin(), cur->second.arr.end(), sym) != cur->second.arr.end())
        {
            grm.grm.erase(cur);
        }
    }
}


void NoAvailableContextFreeGrammar::run(grm_t & grm)
{
    make_good_set(grm);
    collect_non_terminal(grm);
    make_bad_set();
    do_delete(grm);
}

void NoAvailableContextFreeGrammar::make_good_set(const grm_t & grm)
{
    cur_set.insert(grm.start.begin(), grm.start.end());

    while (!cur_set.empty())
    {
        good_set.insert(cur_set.begin(), cur_set.end());

        for (Symbol sym : cur_set)
        {
            do_step(grm, sym);
        }

        cur_set = temp_set;
        temp_set.clear();
    }
}

void NoAvailableContextFreeGrammar::do_step(const grm_t & grm, Symbol target)
{
    auto range = grm.grm.equal_range(target);

    for (auto it = range.first; it != range.second; it++)
    {
        for (Symbol sym : it->second.arr)
        {
            if (sym.kind == NOT_TERM && good_set.find(sym) == good_set.end())
            {
                temp_set.insert(sym);
            }
        }
    }
}

void NoAvailableContextFreeGrammar::collect_non_terminal(const grm_t & grm)
{
    for (auto & rule : grm.grm)
    {
        non_terminals.insert(rule.first);
        collect_str_non_terminal(rule.second);
    }
}

void
NoAvailableContextFreeGrammar::collect_str_non_terminal(const sequence_t & chain)
{
    for (Symbol sym : chain.arr)
    {
        if (sym.kind == NOT_TERM)
        {
            non_terminals.insert(sym);
        }
    }
}

void NoAvailableContextFreeGrammar::make_bad_set()
{
    std::set_difference(non_terminals.begin(), non_terminals.end(),
                        good_set.begin(), good_set.end(),
                        std::inserter(bad_set, bad_set.begin()));
}

void NoAvailableContextFreeGrammar::do_delete(grm_t & grm)
{
    for (Symbol sym : bad_set)
    {
        do_delete_sym(grm, sym);
    }
}

void NoAvailableContextFreeGrammar::do_delete_sym(grm_t & grm, Symbol sym)
{
    for (auto it = grm.grm.begin(); it != grm.grm.end();)
    {
        auto cur = it;
        it++;

        if (cur->first == sym ||
        std::find(cur->second.arr.begin(), cur->second.arr.end(), sym) != cur->second.arr.end())
        {
            grm.grm.erase(cur);
        }
    }
}


void ProductionContextFreeGrammar::run(grm_t & grm)
{
    no_child.run(grm);
    no_ava.run(grm);
}
