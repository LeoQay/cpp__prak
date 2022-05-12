#ifndef CPP__PRAK_GRM_PRODUCTION_H
#define CPP__PRAK_GRM_PRODUCTION_H


#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <vector>


typedef std::pair<std::string, std::string> rule_t;
typedef std::multimap<std::string, std::string> grm_t;
typedef std::unordered_set<std::string> set_t;
typedef std::set<char> set_sym_t;
typedef std::vector<std::string> stack_t;


class NoChildContextFreeGrammar
{
public:
    void run(grm_t & grm);

private:
    void build_N(const grm_t & grm);
    bool do_step(const grm_t & grm);

    bool is_has_child_str(const std::string & str);
    bool is_has_child(char non_terminal);

    void collect_non_terminal(const grm_t & grm);
    void collect_str_non_terminal(const std::string & str);

    void make_bad_set();

    void do_delete(grm_t & grm);
    void do_delete_sym(grm_t & grm, char sym);

    void clear();

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
    void do_step(const grm_t & grm, std::string & str);

    void collect_non_terminal(const grm_t & grm);
    void collect_str_non_terminal(const std::string & str);

    void do_delete(grm_t & grm);
    void do_delete_sym(grm_t & grm, char sym);

    void clear();

    char start ='T';
    set_sym_t good_set = {};
    set_sym_t bad_set = {};
    set_sym_t temp_set = {};
    set_sym_t cur_set = {};
    set_sym_t non_terminals = {};
};


class ProductionContextFreeGrammar
{
    /*
     * Class works only with context-free grammars
     */
public:
    ProductionContextFreeGrammar();
    void run(grm_t & grm);
private:
    NoChildContextFreeGrammar no_child;
    NoAvailableContextFreeGrammar no_ava;
};

ProductionContextFreeGrammar::ProductionContextFreeGrammar()
: no_child(), no_ava() {}

void ProductionContextFreeGrammar::run(grm_t & grm)
{
    no_child.run(grm);
    no_ava.run(grm);
}



#endif //CPP__PRAK_GRM_PRODUCTION_H
