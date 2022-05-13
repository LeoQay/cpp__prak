#ifndef CPP__PRAK_DELETE_EMPTY_RULES_H
#define CPP__PRAK_DELETE_EMPTY_RULES_H


#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <vector>


typedef std::pair<std::string, std::string> rule_t;
typedef std::multimap<std::string, std::string> grm_t;
typedef std::unordered_set<std::string> set_t;
typedef std::vector<std::string> stack_t;


class NoShortContextFreeGrammar
{
    /*
     * Class works only with context-free grammars
     */
public:
    NoShortContextFreeGrammar() = default;
    ~NoShortContextFreeGrammar() = default;

    void run(grm_t & grm);

private:
    // step 1
    void build_X(const grm_t & grm);
    // step 2
    void delete_empty_rules(grm_t & grm);
    bool all_in_X(const std::string & str);
    // step 3
    void add_new_start_sym(grm_t & grm);
    void rename_start_sym(grm_t & grm);
    void replace_start_sym(std::string & str);
    void add_new_start_rules(grm_t & grm);
    // step 4
    void update_rules(grm_t & grm);
    void add_all_combinations(grm_t & grm, const rule_t & rule);
    std::string make_combination(grm_t & grm,
                                 stack_t & alpha,
                                 stack_t & from_X,
                                 int mask);
    std::string parse_alpha(std::string & right);
    std::string parse_X(std::string & right);

    void clear();

    std::string old_start = "S";
    std::string new_start = "T";
    set_t X = {};
};



#endif //CPP__PRAK_DELETE_EMPTY_RULES_H
