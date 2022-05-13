#include <string>
#include <map>
#include <set>

#include "left_grm_to_dfa.h"


void LeftDFAMgr::build_dfa(const grm_t & grm)
{
    collect_terminals(grm);

    sequence_t seq;
    seq.arr.push_back(start);
    que.insert(seq);

    while (!que.empty())
    {
        do_step(grm);
    }

    get_new_names();
    convert_dfa_to_grm();
    collect_finite_not_terminals(grm);
}


void LeftDFAMgr::do_step(const grm_t & grm)
{
    sequence_t current = *que.begin();
    que.erase(current);

    for (Symbol terminal : terminals)
    {
        sequence_t value = get_all_values(grm, current, terminal);

        if (!value.empty())
        {
            dfa.emplace(value, dfa_rule_t(current, terminal));

            if (watched.find(value) == watched.end() && value != current)
            {
                que.insert(value);
            }
        }
    }

    watched.insert(current);
}

sequence_t LeftDFAMgr::get_all_values(const grm_t & grm,
                                      const sequence_t & vertex,
                                      Symbol terminal)
{
    std::set<Symbol> values;

    for (auto & sym : vertex.arr)
    {
        std::set<Symbol> cur = get_non_terminal_all_values(grm, sym, terminal);
        values.insert(cur.begin(), cur.end());
    }

    sequence_t ret;
    for (auto & sym : values)
    {
        ret.arr.push_back(sym);
    }
    return ret;
}

std::set<Symbol>
LeftDFAMgr::get_non_terminal_all_values(const grm_t & grm,
                                        Symbol non_terminal,
                                        Symbol terminal)
{
    std::set<Symbol> ret;

    for (auto & [nt, value] : grm.grm)
    {
        if ((value.size() == 1 && non_terminal == start && value.arr[0] == terminal) ||
            (value.size() == 2 && value.arr[0] == non_terminal && value.arr[1] == terminal))
        {
            ret.insert(nt);
        }
    }

    return ret;
}

void LeftDFAMgr::collect_terminals(const grm_t & grm)
{
    for (auto & rule : grm.grm)
    {
        for (auto & sym : rule.second.arr)
        {
            if (sym.kind == TERM)
            {
                terminals.insert(sym);
            }
        }
    }
}

void LeftDFAMgr::convert_dfa_to_grm()
{
    for (auto & [left, right] : dfa)
    {
        sequence_t temp;
        if (!(right.first.size() == 1 && right.first.arr[0].is_H()))
        {
            temp.arr.push_back(new_names[right.first]);
        }
        temp.arr.push_back(right.second);

        result.grm.emplace(new_names[left], temp);
    }
}

void LeftDFAMgr::get_new_names()
{
    for (auto & nt : watched)
    {
        if (nt.arr.size() == 1)
        {
            new_names[nt] = nt.arr[0];
        }
        else
        {
            new_names[nt] = Symbol(NOT_TERM, Symbol::next_name());
        }
    }
}

void LeftDFAMgr::collect_finite_not_terminals(const grm_t & grm)
{
    for (auto & name : watched)
    {
        std::set<Symbol> sims(name.arr.begin(), name.arr.end());
        std::vector<Symbol> temp;
        std::set_intersection(sims.begin(), sims.end(),
                              grm.start.begin(), grm.start.end(),
                              std::inserter(temp, temp.begin()));
        if (!temp.empty())
        {
            result.start.insert(new_names[name]);
        }
    }
}
