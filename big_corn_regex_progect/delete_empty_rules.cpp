#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <vector>

#include "delete_empty_rules.h"



void NoShortContextFreeGrammar::run(grm_t & grm)
{
    clear();

    build_X(grm);

    delete_empty_rules(grm);

    add_new_start_sym(grm);

    update_rules(grm);
}

void NoShortContextFreeGrammar::build_X(const grm_t & grm)
{
    int count = 1;

    while (count > 0)
    {
        count = 0;

        for (auto it = grm.begin(); it != grm.end();)
        {
            auto cur = it;
            it++;

            if (X.find(cur->first) == X.end() &&
                (cur->second.empty() || all_in_X(cur->second)))
            {
                count++;
                X.insert(cur->first);
            }
        }
    }
}

void NoShortContextFreeGrammar::delete_empty_rules(grm_t & grm)
{
    for (auto it = grm.begin(); it != grm.end();)
    {
        auto cur = it;
        it++;

        if (cur->second.empty())
        {
            grm.erase(cur);
        }
    }
}

bool NoShortContextFreeGrammar::all_in_X(const std::string & str)
{
    return std::all_of(str.begin(), str.end(),
                       [this] (char sym)
                       {
                           std::string s = "_";
                           s[0] = sym;
                           return X.find(s) != X.end();
                       });
}

void NoShortContextFreeGrammar::add_new_start_sym(grm_t & grm)
{
    if (X.find(old_start) == X.end())
    {
        rename_start_sym(grm);
    }
    else
    {
        add_new_start_rules(grm);
    }
}

void NoShortContextFreeGrammar::add_new_start_rules(grm_t & grm)
{
    grm.insert(rule_t(new_start, old_start));
    grm.insert(rule_t(new_start, ""));
}

void NoShortContextFreeGrammar::rename_start_sym(grm_t & grm)
{
    for (auto & rule : grm)
    {
        replace_start_sym(const_cast<std::string &>(rule.first));
        replace_start_sym(const_cast<std::string &>(rule.second));
    }
}

void NoShortContextFreeGrammar::replace_start_sym(std::string & str)
{
    for (char & sym : str)
    {
        if (sym == old_start[0])
        {
            sym = new_start[0];
        }
    }
}

void NoShortContextFreeGrammar::update_rules(grm_t & grm)
{
    grm_t new_grm;

    for (auto & it : grm)
    {
        add_all_combinations(new_grm, it);
    }

    grm = new_grm;
}

void NoShortContextFreeGrammar::add_all_combinations(grm_t & grm, const rule_t & rule)
{
    std::vector<std::string> alpha;
    std::vector<std::string> from_X;

    std::string right = rule.second;

    alpha.push_back(parse_alpha(right));
    if (right.empty())
    {
        grm.insert(rule);
        return;
    }

    while (!right.empty())
    {
        from_X.push_back(parse_X(right));
        alpha.push_back(parse_alpha(right));
    }

    bool all_empty = std::all_of(alpha.begin(), alpha.end(),
                                 [] (std::string & s)
                                 { return s.empty(); });

    int start = all_empty ? 1 : 0;
    int amount = 1 << from_X.size();

    for (int i = start; i < amount; i++)
    {
        grm.insert(
        rule_t(rule.first, make_combination(grm, alpha, from_X, i))
        );
    }
}

std::string NoShortContextFreeGrammar::parse_alpha(std::string & right)
{
    size_t pos = 0, len = right.size();

    while (pos < len && X.find(right.substr(pos, 1)) == X.end())
    {
        pos++;
    }

    std::string ret = right.substr(0, pos);
    right.erase(0, pos);
    return ret;
}

std::string NoShortContextFreeGrammar::parse_X(std::string & right)
{
    if (right.empty())
    {
        throw std::exception();
    }
    std::string ret = right.substr(0, 1);

    if (X.find(ret) == X.end())
    {
        throw std::exception();
    }

    right.erase(0, 1);
    return ret;
}

std::string NoShortContextFreeGrammar::make_combination(grm_t & grm,
                                                        stack_t & alpha,
                                                        stack_t & from_X,
                                                        int mask)
{
    size_t len = from_X.size();
    size_t alpha_len = alpha.size();

    if (len == 0)
    {
        // if in string doesn't exist non-terminals from X
        // this method couldn't be launched
        throw std::exception();
    }

    if (len + 1 != alpha_len)
    {
        // logic error, wrong stacks
        throw std::exception();
    }

    std::string result;

    for (size_t i = 0; i < len; i++)
    {
        result += alpha[i];
        if ((mask & 1) == 1)
        {
            result += from_X[i];
        }
        mask >>= 1;
    }

    result += alpha[len];
    return result;
}

void NoShortContextFreeGrammar::clear()
{
    X.clear();
}

