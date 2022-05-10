#ifndef CPP_PRAK_92_HFILE_H
#define CPP_PRAK_92_HFILE_H

#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>


typedef std::multimap<char, std::string> grm_t;
typedef std::set<char> char_set;
typedef std::set<std::string> str_set;
typedef std::unordered_map<
std::string,
std::set<char>
> stat_t;
typedef std::unordered_map<
char,
std::set<char>
> char_stat_t;

/*
 * Grammar validity to Recursive Descent Method
 */
class GrmValidityRDM
{
public:
    GrmValidityRDM();
    GrmValidityRDM(const GrmValidityRDM & other) = delete;
    GrmValidityRDM & operator= (const GrmValidityRDM & other) = delete;

    bool check(const grm_t & _grm);
private:
    bool check_first_rule();
    bool check_second_rule();
    bool check_third_rule();

    void build_first();
    void build_first_chain(const std::string & chain, char_set & cont);
    void build_first_non_terminal(char sym, char_set & cont);

    void build_follow();
    void build_follow_non_terminal(char non_terminal, char_set & cont);
    char_set build_follow_sub_set(char non_terminal);

    void build_to_empty();
    bool is_empty_str(const std::string & str);

    void build_non_terminals();
    void build_terminals();

    void clear();

    static bool is_non_terminal(char sym);

    static std::string to_str(char sym);

    const grm_t * grm;
    stat_t first_stat;

    char_stat_t follow_stat;

    // множество не терминалов задействованных в рекурсии
    // для предотвращения зацикливания
    str_set pool;
    char_set char_pool;

    char_set to_empty;
    char_set non_terminals;
    char_set terminals;
};

#endif //CPP_PRAK_92_HFILE_H
