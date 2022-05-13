#ifndef CPP__PRAK_DELETE_EMPTY_RULES_H
#define CPP__PRAK_DELETE_EMPTY_RULES_H


#include <string>
#include <map>
#include <set>
#include <vector>

#include "global_typedef.h"


class DeleteEmptyRules
{
public:
    void run(grm_t & grm);
private:
    // step 1
    void build_X(const grm_t & grm);
    // step 2
    void delete_empty_rules(grm_t & grm);
    bool all_in_X(const sequence_t & chain);
    // step 3

    // step 4
    void update_rules(grm_t & grm);
    void add_all_combinations(grm_t & grm, const pair_t & rule);
    sequence_t make_combination(grm_t & grm,
                                std::vector<sequence_t> & alpha,
                                std::vector<sequence_t> & from_X,
                                int mask);
    sequence_t parse_alpha(sequence_t & right);
    static sequence_t parse_X(sequence_t & right);

    std::set<Symbol> X;
};



#endif //CPP__PRAK_DELETE_EMPTY_RULES_H
