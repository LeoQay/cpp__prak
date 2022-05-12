#ifndef CPP__PRAK_LEFT_GRM_TO_DFA_H
#define CPP__PRAK_LEFT_GRM_TO_DFA_H


#include <string>
#include <map>
#include <unordered_map>
#include <set>


using grm_t = std::multimap<std::string, std::string>;
using dfa_t = std::multimap<std::string, std::pair<std::string, char>>;
using dfa_rule_t = std::pair<std::string, char>;
using dfa_ds_t = std::unordered_map<std::string, std::unordered_map<char, std::string>>;


class LeftDFAMgr
{
public:
    explicit LeftDFAMgr(const grm_t & grm);
    virtual ~LeftDFAMgr() = default;
    bool check(const std::string & str);
private:
    void convert_dfa_to_ds();

    void do_step(const grm_t & grm);

    std::string get_all_values(const grm_t & grm,
                               const std::string & vertex,
                               char terminal);

    std::set<char> get_non_terminal_all_values(const grm_t & grm,
                                               char non_terminal,
                                               char terminal);

    void collect_terminals(const grm_t & grm);

    dfa_t dfa;
    dfa_ds_t ds;

    std::string start_sym = "H";

    // не терминалы или их последовательности
    // записываются сюда после просмотра
    std::set<std::string> watched;
    // очередь не терминалов на просмотр
    std::set<std::string> que;
    // конечные не терминалы
    std::set<std::string> finite;

    // терминалы
    std::set<char> terminals;
};


#endif //CPP__PRAK_LEFT_GRM_TO_DFA_H
