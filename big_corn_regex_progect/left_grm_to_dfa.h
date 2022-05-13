#ifndef CPP__PRAK_LEFT_GRM_TO_DFA_H
#define CPP__PRAK_LEFT_GRM_TO_DFA_H

#include <string>
#include <map>
#include <set>

#include "global_typedef.h"


using dfa_t = std::multimap<sequence_t, std::pair<sequence_t, Symbol>>;
using dfa_rule_t = std::pair<sequence_t, Symbol>;
using dfa_ds_t = std::map<sequence_t, std::map<Symbol, sequence_t>>;


class LeftDFAMgr
{
public:
    grm_t build_dfa(const grm_t & grm);
private:
    void convert_dfa_to_ds();

    void do_step(const grm_t & grm);

    sequence_t get_all_values(const grm_t & grm,
                              const sequence_t & vertex,
                              Symbol terminal);

    std::set<Symbol> get_non_terminal_all_values(const grm_t & grm,
                                                Symbol non_terminal,
                                                Symbol terminal);

    void collect_terminals(const grm_t & grm);

    Symbol start = Symbol(NOT_TERM, 0);

    dfa_t dfa;
    dfa_ds_t ds;

    // не терминалы или их последовательности
    // записываются сюда после просмотра
    std::set<sequence_t> watched;
    // очередь не терминалов на просмотр
    std::set<sequence_t> que;
    // конечные не терминалы
    std::set<sequence_t> finite;

    // терминалы
    std::set<Symbol> terminals;
};


#endif //CPP__PRAK_LEFT_GRM_TO_DFA_H
