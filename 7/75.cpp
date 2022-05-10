#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <vector>
#include <algorithm>


typedef std::pair<std::string, std::string> rule_t;
typedef std::multimap<std::string, std::string> grm_t;
typedef std::unordered_set<std::string> set_t;
typedef std::set<char> set_sym_t;
typedef std::vector<std::string> stack_t;

class GrammarTypeChecker
{
public:
    static bool check_sym_not_terminal(char sym);
};

bool GrammarTypeChecker::check_sym_not_terminal(char sym)
{
    return sym >= 'A' && sym <= 'Z';
}


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

void NoShortContextFreeGrammar::run(grm_t & grm)
{
    clear();

    build_X(grm);

    delete_empty_rules(grm);

    add_new_start_sym(grm);

    update_rules(grm);
}

void NoShortContextFreeGrammar::build_X(const grm_t & grm)
{
    int count = 1;

    while (count > 0)
    {
        count = 0;

        for (auto it = grm.begin(); it != grm.end();)
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

void NoShortContextFreeGrammar::delete_empty_rules(grm_t & grm)
{
    for (auto it = grm.begin(); it != grm.end();)
    {
        auto cur = it;
        it++;

        if (cur->second.empty())
        {
            grm.erase(cur);
        }
    }
}

bool NoShortContextFreeGrammar::all_in_X(const std::string & str)
{
    return std::all_of(str.begin(), str.end(),
                       [this] (char sym)
                       {
        std::string s = "_";
        s[0] = sym;
        return X.find(s) != X.end();
                       });
}

void NoShortContextFreeGrammar::add_new_start_sym(grm_t & grm)
{
    if (X.find(old_start) == X.end())
    {
        rename_start_sym(grm);
    }
    else
    {
        add_new_start_rules(grm);
    }
}

void NoShortContextFreeGrammar::add_new_start_rules(grm_t & grm)
{
    grm.insert(rule_t(new_start, old_start));
    grm.insert(rule_t(new_start, ""));
}

void NoShortContextFreeGrammar::rename_start_sym(grm_t & grm)
{
    for (auto & rule : grm)
    {
        replace_start_sym(const_cast<std::string &>(rule.first));
        replace_start_sym(const_cast<std::string &>(rule.second));
    }
}

void NoShortContextFreeGrammar::replace_start_sym(std::string & str)
{
    for (char & sym : str)
    {
        if (sym == old_start[0])
        {
            sym = new_start[0];
        }
    }
}

void NoShortContextFreeGrammar::update_rules(grm_t & grm)
{
    grm_t new_grm;

    for (auto & it : grm)
    {
        add_all_combinations(new_grm, it);
    }

    grm = new_grm;
}

void NoShortContextFreeGrammar::add_all_combinations(grm_t & grm, const rule_t & rule)
{
    std::vector<std::string> alpha;
    std::vector<std::string> from_X;

    std::string right = rule.second;

    alpha.push_back(parse_alpha(right));
    if (right.empty())
    {
        grm.insert(rule);
        return;
    }

    while (!right.empty())
    {
        from_X.push_back(parse_X(right));
        alpha.push_back(parse_alpha(right));
    }

    bool all_empty = std::all_of(alpha.begin(), alpha.end(),
                                 [] (std::string & s)
                                 { return s.empty(); });

    int start = all_empty ? 1 : 0;
    int amount = 1 << from_X.size();

    for (int i = start; i < amount; i++)
    {
        grm.insert(
        rule_t(rule.first, make_combination(grm, alpha, from_X, i))
        );
    }
}

std::string NoShortContextFreeGrammar::parse_alpha(std::string & right)
{
    size_t pos = 0, len = right.size();

    while (pos < len && X.find(right.substr(pos, 1)) == X.end())
    {
        pos++;
    }

    std::string ret = right.substr(0, pos);
    right.erase(0, pos);
    return ret;
}

std::string NoShortContextFreeGrammar::parse_X(std::string & right)
{
    if (right.empty())
    {
        throw std::exception();
    }
    std::string ret = right.substr(0, 1);

    if (X.find(ret) == X.end())
    {
        throw std::exception();
    }

    right.erase(0, 1);
    return ret;
}

std::string NoShortContextFreeGrammar::make_combination(grm_t & grm,
                                                        stack_t & alpha,
                                                        stack_t & from_X,
                                                        int mask)
{
    size_t len = from_X.size();
    size_t alpha_len = alpha.size();

    if (len == 0)
    {
        // if in string doesn't exist non-terminals from X
        // this method couldn't be launched
        throw std::exception();
    }

    if (len + 1 != alpha_len)
    {
        // logic error, wrong stacks
        throw std::exception();
    }

    std::string result;

    for (size_t i = 0; i < len; i++)
    {
        result += alpha[i];
        if ((mask & 1) == 1)
        {
            result += from_X[i];
        }
        mask >>= 1;
    }

    result += alpha[len];
    return result;
}

void NoShortContextFreeGrammar::clear()
{
    X.clear();
}


class SelfToSelfCleanerGrammar
{
public:
    static void run(grm_t & grm);
};

void SelfToSelfCleanerGrammar::run(grm_t & grm)
{
    for (auto it = grm.begin(); it != grm.end();)
    {
        auto cur = it;
        it++;

        if (cur->first == cur->second)
        {
            grm.erase(cur);
        }
    }
}


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

void NoChildContextFreeGrammar::run(grm_t & grm)
{
    clear();
    build_N(grm);
    collect_non_terminal(grm);
    make_bad_set();
    do_delete(grm);
}

void NoChildContextFreeGrammar::build_N(const grm_t & grm)
{
    while (do_step(grm));
}

bool NoChildContextFreeGrammar::do_step(const grm_t & grm)
{
    for (auto & rule : grm)
    {
        if (is_has_child_str(rule.second))
        {
            cur_N.insert(rule.first[0]);
        }
    }

    if (cur_N.size() == prev_N.size())
    {
        return false;
    }

    prev_N.insert(cur_N.begin(), cur_N.end());
    cur_N.clear();

    return true;
}

bool NoChildContextFreeGrammar::is_has_child_str(const std::string & str)
{
    return std::all_of(str.begin(), str.end(),
                       [this] (char sym) { return is_has_child(sym); } );
}

bool NoChildContextFreeGrammar::is_has_child(char non_terminal)
{
    return !GrammarTypeChecker::check_sym_not_terminal(non_terminal) ||
           prev_N.find(non_terminal) != prev_N.end();
}

void NoChildContextFreeGrammar::collect_non_terminal(const grm_t & grm)
{
    for (auto & rule : grm)
    {
        collect_str_non_terminal(rule.first);
        collect_str_non_terminal(rule.second);
    }
}

void
NoChildContextFreeGrammar::collect_str_non_terminal(const std::string & str)
{
    for (char sym : str)
    {
        if (GrammarTypeChecker::check_sym_not_terminal(sym))
        {
            non_terminals.insert(sym);
        }
    }
}

void NoChildContextFreeGrammar::make_bad_set()
{
    std::set_difference(non_terminals.begin(), non_terminals.end(),
                        prev_N.begin(), prev_N.end(),
                        std::inserter(bad_set, bad_set.begin()));
}

void NoChildContextFreeGrammar::clear()
{
    prev_N.clear();
    cur_N.clear();
    non_terminals.clear();
    bad_set.clear();
}

void NoChildContextFreeGrammar::do_delete(grm_t & grm)
{
    for (char sym : bad_set)
    {
        do_delete_sym(grm, sym);
    }
}

void NoChildContextFreeGrammar::do_delete_sym(grm_t & grm, char sym)
{
    for (auto it = grm.begin(); it != grm.end();)
    {
        auto cur = it;
        it++;

        if (static_cast<int>(cur->first.find(sym)) != -1 ||
            static_cast<int>(cur->second.find(sym)) != -1)
        {
            grm.erase(cur);
        }
    }
}


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

void NoAvailableContextFreeGrammar::run(grm_t & grm)
{
    clear();
    make_good_set(grm);
    collect_non_terminal(grm);
    make_bad_set();
    do_delete(grm);
}

void NoAvailableContextFreeGrammar::make_good_set(const grm_t & grm)
{
    cur_set.insert(start);

    while (!cur_set.empty())
    {
        good_set.insert(cur_set.begin(), cur_set.end());

        std::string s = "_";
        for (char sym : cur_set)
        {
            s[0] = sym;
            do_step(grm, s);
        }

        cur_set = temp_set;
        temp_set.clear();
    }
}

void
NoAvailableContextFreeGrammar::do_step(const grm_t & grm, std::string & str)
{
    auto range = grm.equal_range(str);

    for (auto it = range.first; it != range.second; it++)
    {
        for (char sym : it->second)
        {
            if (GrammarTypeChecker::check_sym_not_terminal(sym) &&
            good_set.find(sym) == good_set.end())
            {
                temp_set.insert(sym);
            }
        }
    }
}

void NoAvailableContextFreeGrammar::collect_non_terminal(const grm_t & grm)
{
    for (auto & rule : grm)
    {
        collect_str_non_terminal(rule.first);
        collect_str_non_terminal(rule.second);
    }
}

void
NoAvailableContextFreeGrammar::collect_str_non_terminal(const std::string & str)
{
    for (char sym : str)
    {
        if (GrammarTypeChecker::check_sym_not_terminal(sym))
        {
            non_terminals.insert(sym);
        }
    }
}

void NoAvailableContextFreeGrammar::clear()
{
    bad_set.clear();
    good_set.clear();
    temp_set.clear();
    cur_set.clear();
    non_terminals.clear();
}

void NoAvailableContextFreeGrammar::make_bad_set()
{
    std::set_difference(non_terminals.begin(), non_terminals.end(),
                        good_set.begin(), good_set.end(),
                        std::inserter(bad_set, bad_set.begin()));
}

void NoAvailableContextFreeGrammar::do_delete(grm_t & grm)
{
    for (char sym : bad_set)
    {
        do_delete_sym(grm, sym);
    }
}

void NoAvailableContextFreeGrammar::do_delete_sym(grm_t & grm, char sym)
{
    for (auto it = grm.begin(); it != grm.end();)
    {
        auto cur = it;
        it++;

        if (static_cast<int>(cur->first.find(sym)) != -1 ||
            static_cast<int>(cur->second.find(sym)) != -1)
        {
            grm.erase(cur);
        }
    }
}


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


int main()
{
    //std::ifstream fin("file");

    std::set<rule_t> prev_grm;
    grm_t grm;

    std::string str;
    while(getline(std::cin, str))
    {
        size_t sep_pos = str.find(':');
        prev_grm.insert(
        rule_t(str.substr(0, sep_pos), str.substr(sep_pos + 1))
        );
    }

    for (auto & rule : prev_grm)
    {
        grm.insert(rule);
    }

    NoShortContextFreeGrammar no_shorter;
    no_shorter.run(grm);

    ProductionContextFreeGrammar producer;
    producer.run(grm);

    SelfToSelfCleanerGrammar::run(grm);

    std::set<std::string> answer;

    for (auto & it : grm)
    {
        answer.insert(it.first + ":" + it.second);
    }

    for (auto & it : answer)
    {
        std::cout << it << std::endl;
    }

    //fin.close();

    return 0;
}
