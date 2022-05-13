#ifndef CPP__PRAK_DELETE_BAD_H
#define CPP__PRAK_DELETE_BAD_H

#include <set>

#include "global_typedef.h"


using set_sym_t = std::set<Symbol>;


class NoChildContextFreeGrammar
{
public:
    void run(grm_t & grm);
private:
    void build_N(const grm_t & grm);
    bool do_step(const grm_t & grm);

    bool is_has_child_str(const sequence_t & chain);
    bool is_has_child(Symbol non_terminal);

    void collect_non_terminal(const grm_t & grm);
    void collect_str_non_terminal(const sequence_t & chain);

    void make_bad_set();

    void do_delete(grm_t & grm);
    void do_delete_sym(grm_t & grm, Symbol sym);

    set_sym_t bad_set = {};
    set_sym_t prev_N = {};
    set_sym_t cur_N = {};
    set_sym_t non_terminals = {};
};



class NoAvailableContextFreeGrammar
{
public:
    void run(grm_t & grm);
private:
    void make_good_set(const grm_t & grm);
    void make_bad_set();
    void do_step(const grm_t & grm, Symbol target);

    void collect_non_terminal(const grm_t & grm);
    void collect_str_non_terminal(const sequence_t & chain);

    void do_delete(grm_t & grm);
    void do_delete_sym(grm_t & grm, Symbol sym);

    set_sym_t good_set = {};
    set_sym_t bad_set = {};
    set_sym_t temp_set = {};
    set_sym_t cur_set = {};
    set_sym_t non_terminals = {};
};


class ProductionContextFreeGrammar
{
public:
    void run(grm_t & grm);
private:
    NoChildContextFreeGrammar no_child;
    NoAvailableContextFreeGrammar no_ava;
};


#endif //CPP__PRAK_DELETE_BAD_H
