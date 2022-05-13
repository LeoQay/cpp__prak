#include <iostream>
#include <string>
#include <map>

#include "regex_checker.h"
#include "global_typedef.h"
#include "left_grm_to_dfa.h"
#include "delete_empty_rules.h"
#include "delete_bad.h"


grm_t g()
{
    grm_t sm;
    sm.start.emplace(NOT_TERM, 'S');

    sequence_t seq;
    seq.arr.emplace_back(NOT_TERM, 'S');
    seq.arr.emplace_back(TERM, 'a');
    sm.grm.emplace(Symbol(NOT_TERM, 'S'), seq);
    seq.arr.clear();

    seq.arr.emplace_back(NOT_TERM, 'A');
    seq.arr.emplace_back(TERM, 'b');
    sm.grm.emplace(Symbol(NOT_TERM, 'S'), seq);
    seq.arr.clear();

    seq.arr.emplace_back(TERM, 'a');
    sm.grm.emplace(Symbol(NOT_TERM, 'S'), seq);
    seq.arr.clear();

    seq.arr.emplace_back(NOT_TERM, 'S');
    seq.arr.emplace_back(TERM, 'a');
    sm.grm.emplace(Symbol(NOT_TERM, 'A'), seq);
    seq.arr.clear();

    return sm;
}


int main()
{
    LeftDFAMgr mgr;
    mgr.build_dfa(g());
    grm_t grm = mgr.result;
    DeleteEmptyRules del;
    del.run(grm);
    ProductionContextFreeGrammar producer;
    producer.run(grm);

    std::string current;
    if (!getline(std::cin, current))
    {
        return 0;
    }

    try
    {
        RegexChecker checker(current);

        while (getline(std::cin, current))
        {
            std::cout << (checker.check(current) ? "YES" : "NO") << std::endl;
        }
    }
    catch (std::exception & err)
    {
        std::cout << err.what() << std::endl;
    }

    return 0;
}
