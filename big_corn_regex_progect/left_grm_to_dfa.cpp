#include <string>
#include <map>
#include <set>

#include "left_grm_to_dfa.h"


grm_t LeftDFAMgr::build_dfa(const grm_t & grm)
{
    collect_terminals(grm);

    sequence_t seq;
    seq.arr.push_back(start);
    que.insert(seq);

    while (!que.empty())
    {
        do_step(grm);
    }

    convert_dfa_to_ds();


    // i must return left dfa grm
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

void LeftDFAMgr::convert_dfa_to_ds()
{
    for (auto & [dest, src] : dfa)
    {
        ds[src.first][src.second] = dest;
    }
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
