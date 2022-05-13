#ifndef CPP__PRAK_LEFT_GRM_TO_DFA_H
#define CPP__PRAK_LEFT_GRM_TO_DFA_H

#include <string>
#include <map>
#include <set>

#include "global_typedef.h"


using dfa_t = std::multimap<sequence_t, std::pair<sequence_t, Symbol>>;
using dfa_rule_t = std::pair<sequence_t, Symbol>;


class LeftDFAMgr
{
public:
    void build_dfa(const grm_t & grm);
    grm_t result;
private:
    void convert_dfa_to_grm();

    void do_step(const grm_t & grm);

    sequence_t get_all_values(const grm_t & grm,
                              const sequence_t & vertex,
                              Symbol terminal);

    std::set<Symbol> get_non_terminal_all_values(const grm_t & grm,
                                                Symbol non_terminal,
                                                Symbol terminal);

    void collect_terminals(const grm_t & grm);

    void get_new_names();

    void collect_finite_not_terminals(const grm_t & grm);

    Symbol start = Symbol(NOT_TERM, 0);

    dfa_t dfa;

    // не терминалы или их последовательности
    // записываются сюда после просмотра
    std::set<sequence_t> watched;
    std::map<sequence_t, Symbol> new_names;

    // очередь не терминалов на просмотр
    std::set<sequence_t> que;

    // терминалы
    std::set<Symbol> terminals;
};


#endif //CPP__PRAK_LEFT_GRM_TO_DFA_H
