#include "92_hfile.h"

GrmValidityRDM::GrmValidityRDM() : grm(nullptr), first_stat(),
                                   follow_stat(), pool(), char_pool(),
                                   to_empty(), non_terminals(), terminals() {}


bool GrmValidityRDM::check(const grm_t & _grm)
{
    grm = &_grm;

    build_non_terminals();
    build_terminals();
    build_to_empty();

    if (!check_second_rule())
    {
        clear();
        return false;
    }

    build_first();

    if (!check_first_rule())
    {
        clear();
        return false;
    }

    build_follow();

    if (!check_third_rule())
    {
        clear();
        return false;
    }

    clear();
    return true;
}


bool GrmValidityRDM::check_first_rule()
{
    for (char non_terminal : non_terminals)
    {
        char_set mid;

        auto range = grm->equal_range(non_terminal);

        for (auto it = range.first; it != range.second; it++)
        {
            auto & stat = first_stat[it->second];

            size_t prev = mid.size();
            size_t add = stat.size();

            mid.insert(stat.begin(), stat.end());

            if (prev + add != mid.size())
            {
                return false;
            }
        }
    }

    return true;
}

bool GrmValidityRDM::check_second_rule()
{
    for (char non_terminal : non_terminals)
    {
        size_t counter = 0;

        auto range = grm->equal_range(non_terminal);

        for (auto it = range.first; it != range.second; it++)
        {
            if (is_empty_str(it->second))
            {
                counter++;
            }
        }

        if (counter >= 2)
        {
            return false;
        }
    }

    return true;
}

bool GrmValidityRDM::check_third_rule()
{
    for (char non_terminal : to_empty)
    {
        auto & follow = follow_stat[non_terminal];
        auto & first = first_stat[to_str(non_terminal)];

        std::vector<char> result;
        std::set_intersection(follow.begin(), follow.end(),
                              first.begin(), first.end(),
                              std::inserter(result, result.begin()));

        if (!result.empty())
        {
            return false;
        }
    }

    return true;
}

void GrmValidityRDM::build_follow()
{
    follow_stat.clear();

    for (char sym : non_terminals)
    {
        follow_stat[sym] = {};
    }

    for (char sym : non_terminals)
    {
        char_pool.clear();
        build_follow_non_terminal(sym, follow_stat[sym]);
    }
}

void GrmValidityRDM::build_follow_non_terminal(char non_terminal, char_set & cont)
{
    char_pool.insert(non_terminal);

    // множество таких не терминалов, что они выводят некоторую цепочку,
    // в которой целевой не терминал стоит на правом краю
    char_set sub_set = build_follow_sub_set(non_terminal);

    for (char sym : sub_set)
    {
        if (char_pool.find(sym) != char_pool.end())
        {
            continue;
        }

        build_follow_non_terminal(sym, cont);
    }


    for (auto & [l, r] : *grm)
    {
        size_t pos = 0;
        while (true)
        {
            pos = r.find(non_terminal, pos);

            if (pos >= r.size())
            {
                break;
            }

            pos++;

            build_first_chain(r.substr(pos), cont);
        }
    }

    char_pool.erase(non_terminal);
}

char_set GrmValidityRDM::build_follow_sub_set(char non_terminal)
{
    char_set result;
    result.insert(non_terminal);

    size_t prev = -1;

    while (prev != result.size())
    {
        prev = result.size();

        for (auto & [l, r] : *grm)
        {
            if (result.find(l) != result.end() || r.empty())
            {
                continue;
            }

            size_t pos = r.size();

            while (true)
            {
                char right = r[--pos];

                if (result.find(right) != result.end() &&
                    is_empty_str(r.substr(pos + 1)))
                {
                    result.insert(l);
                }

                if (pos == 0)
                {
                    break;
                }
            }

        }
    }

    return result;
}



void GrmValidityRDM::build_first()
{
    for (auto & [l, r] : *grm)
    {
        pool.clear();

        build_first_non_terminal(l, first_stat[to_str(l)]);
        build_first_chain(r, first_stat[r]);
    }
}

void GrmValidityRDM::build_first_non_terminal(char sym, char_set & cont)
{
    auto str = to_str(sym);

    pool.insert(str);

    auto range = grm->equal_range(sym);

    for (auto it = range.first; it != range.second; it++)
    {
        if (pool.find(it->second) != pool.end())
        {
            continue;
        }

        build_first_chain(it->second, cont);
    }

    pool.erase(str);
}

void GrmValidityRDM::build_first_chain(const std::string & chain, char_set & cont)
{
    size_t pos = 0;
    size_t len = chain.length();

    while (true)
    {
        if (pos >= len)
        {
            break;
        }

        char sym = chain[pos];
        auto str = to_str(sym);

        if (!is_non_terminal(sym))
        {
            cont.insert(sym);
            break;
        }

        if (pool.find(str) != pool.end())
        {
            if (to_empty.find(sym) != to_empty.end())
            {
                pos++;
                continue;
            }

            return;
        }

        build_first_non_terminal(sym, cont);

        if (to_empty.find(sym) == to_empty.end())
        {
            break;
        }

        pos++;
    }
}


bool GrmValidityRDM::is_non_terminal(char sym)
{
    return sym >= 'A' && sym <= 'Z';
}


void GrmValidityRDM::build_to_empty()
{
    to_empty.clear();

    size_t prev_size = -1, cur_size = 0;

    while (cur_size != prev_size)
    {
        prev_size = cur_size;

        for (auto & [l, r] : *grm)
        {
            if (is_empty_str(r))
            {
                to_empty.insert(l);
            }
        }

        cur_size = to_empty.size();
    }
}

bool GrmValidityRDM::is_empty_str(const std::string & str)
{
    return str.empty() || std::all_of(str.begin(), str.end(),
                                      [this] (char sym)
                                      {return to_empty.find(sym) != to_empty.end();});
}

std::string GrmValidityRDM::to_str(char sym)
{
    std::string s = "_";
    s[0] = sym;
    return s;
}

void GrmValidityRDM::build_non_terminals()
{
    non_terminals.clear();

    for (auto & [l, r] : *grm)
    {
        non_terminals.insert(l);
    }
}

void GrmValidityRDM::build_terminals()
{
    terminals.clear();

    for (auto & [l, r] : *grm)
    {
        for (char sym : r)
        {
            if (!is_non_terminal(sym))
            {
                terminals.insert(sym);
            }
        }
    }
}

void GrmValidityRDM::clear()
{
    grm = nullptr;
    first_stat.clear();
    follow_stat.clear();
    pool.clear();
    char_pool.clear();
    to_empty.clear();
    non_terminals.clear();
    terminals.clear();
}
