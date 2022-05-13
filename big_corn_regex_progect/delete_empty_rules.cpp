#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <vector>

#include "delete_empty_rules.h"



void DeleteEmptyRules::run(grm_t & grm)
{
    clear();

    build_X(grm);

    delete_empty_rules(grm);



    update_rules(grm);
}

void DeleteEmptyRules::build_X(const grm_t & grm)
{
    int count = 1;

    while (count > 0)
    {
        count = 0;

        for (auto it = grm.grm.begin(); it != grm.grm.end();)
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

void DeleteEmptyRules::delete_empty_rules(grm_t & grm)
{
    for (auto it = grm.grm.begin(); it != grm.grm.end();)
    {
        auto cur = it;
        it++;

        if (cur->second.empty())
        {
            grm.grm.erase(cur);
        }
    }
}

bool DeleteEmptyRules::all_in_X(const sequence_t & chain)
{
    return std::all_of(chain.arr.begin(), chain.arr.end(),
                       [this] (Symbol sym)
                       {
                           return X.find(sym) != X.end();
                       });
}

void DeleteEmptyRules::update_rules(grm_t & grm)
{
    grm_t new_grm;

    for (auto & it : grm)
    {
        add_all_combinations(new_grm, it);
    }

    grm = new_grm;
}

void DeleteEmptyRules::add_all_combinations(grm_t & grm, const pair_t & rule)
{
    std::vector<sequence_t> alpha;
    std::vector<sequence_t> from_X;

    sequence_t right = rule.second;

    alpha.push_back(parse_alpha(right));
    if (right.empty())
    {
        grm.grm.insert(rule);
        return;
    }

    while (!right.empty())
    {
        from_X.push_back(parse_X(right));
        alpha.push_back(parse_alpha(right));
    }

    bool all_empty = std::all_of(alpha.begin(), alpha.end(),
                                 [] (sequence_t & s)
                                 { return s.empty(); });

    int start = all_empty ? 1 : 0;
    int amount = 1 << from_X.size();

    for (int i = start; i < amount; i++)
    {
        grm.grm.emplace(
            rule.first, make_combination(grm, alpha, from_X, i)
        );
    }
}

sequence_t DeleteEmptyRules::parse_alpha(sequence_t & right)
{
    size_t pos = 0, len = right.size();

    while (pos < len && X.find(right.arr[pos]) == X.end())
    {
        pos++;
    }

    sequence_t ret = right.substr(0, pos);
    right.erase(0, pos);
    return ret;
}

sequence_t DeleteEmptyRules::parse_X(sequence_t & right)
{
    sequence_t ret = right.substr(0, 1);
    right.erase(0, 1);
    return ret;
}

sequence_t DeleteEmptyRules::make_combination(
        grm_t & grm,
        std::vector<sequence_t> & alpha,
        std::vector<sequence_t> & from_X,
        int mask
)
{
    size_t len = from_X.size();
    size_t alpha_len = alpha.size();

    sequence_t result;

    for (size_t i = 0; i < len; i++)
    {
        result.arr.insert(result.arr.end(), alpha[i].arr.begin(), alpha[i].arr.end());
        if ((mask & 1) == 1)
        {
            result.arr.insert(result.arr.end(), from_X[i].arr.begin(), from_X[i].arr.end());
        }
        mask >>= 1;
    }

    result.arr.insert(result.arr.end(), alpha[len].arr.begin(), alpha[len].arr.end());
    return result;
}

