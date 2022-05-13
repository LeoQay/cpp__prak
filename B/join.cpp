#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>

#include "regex_parser.h"


struct Symbol
{
    static int global_name_counter;
    static int next_name();

    enum class SymbolType { TERMINAL, NOT_TERMINAL };

    SymbolType kind;
    int repr;

    bool operator> (const Symbol & other) const;
    bool operator== (const Symbol & other) const;
    bool operator>= (const Symbol & other) const;
    bool operator<= (const Symbol & other) const;
    bool operator< (const Symbol & other) const;
    bool operator!= (const Symbol & other) const;

    [[nodiscard]] bool is_H() const;
    [[nodiscard]] bool is_empty() const;

    explicit Symbol(char sym);
    explicit Symbol(SymbolType kind_ = SymbolType::TERMINAL, int repr_ = '\0');
};


#define TERM Symbol::SymbolType::TERMINAL
#define NOT_TERM Symbol::SymbolType::NOT_TERMINAL



struct RightRuleContextFreeGrm
{
    RightRuleContextFreeGrm();

    std::vector<Symbol> arr;

    [[nodiscard]] std::size_t size()const;
    [[nodiscard]] bool empty() const;

    [[nodiscard]] std::size_t find(const Symbol & sym) const;

    void erase(std::size_t pos, std::size_t n);
    [[nodiscard]] RightRuleContextFreeGrm substr(std::size_t pos, std::size_t n) const;

    bool operator> (const RightRuleContextFreeGrm & other) const;
    bool operator== (const RightRuleContextFreeGrm & other) const;
    bool operator>= (const RightRuleContextFreeGrm & other) const;
    bool operator<= (const RightRuleContextFreeGrm & other) const;
    bool operator< (const RightRuleContextFreeGrm & other) const;
    bool operator!= (const RightRuleContextFreeGrm & other) const;
};


RightRuleContextFreeGrm::RightRuleContextFreeGrm() : arr() {}



struct ContextFreeGrm
{
    ContextFreeGrm();

    std::set<Symbol> start;
    std::multimap<Symbol, RightRuleContextFreeGrm> grm;
};


ContextFreeGrm::ContextFreeGrm() : start(), grm () {}


using sequence_t = RightRuleContextFreeGrm;
using grm_t = ContextFreeGrm;
using pair_t = std::pair<Symbol, RightRuleContextFreeGrm>;
using usefull_dfa_t = std::map<Symbol, std::map<Symbol, Symbol>>;
using set_sym_t = std::set<Symbol>;


int Symbol::global_name_counter = 0;

Symbol::Symbol(char sym) : kind(SymbolType::TERMINAL), repr(sym) {}

Symbol::Symbol(SymbolType kind_, int repr_) : kind(kind_), repr(repr_) {}

int Symbol::next_name()
{
    return ++global_name_counter;
}

bool Symbol::operator== (const Symbol & other) const
{
    return kind == other.kind && repr == other.repr;
}

bool Symbol::operator> (const Symbol & other) const
{
    return repr > other.repr;
}

bool Symbol::operator>= (const Symbol & other) const
{
    return operator>(other) || operator==(other);
}

bool Symbol::operator<= (const Symbol & other) const
{
    return !operator>(other);
}

bool Symbol::operator!= (const Symbol & other) const
{
    return !operator==(other);
}

bool Symbol::operator< (const Symbol & other) const
{
    return !operator>=(other);
}

bool Symbol::is_H() const
{
    return kind == NOT_TERM && repr == 0;
}

bool Symbol::is_empty() const
{
    return kind == TERM && repr == 0;
}


bool RightRuleContextFreeGrm::operator== (const RightRuleContextFreeGrm & other) const
{
    return arr == other.arr;
}

bool RightRuleContextFreeGrm::operator> (const RightRuleContextFreeGrm & other) const
{
    if (arr.size() != other.arr.size()) return arr.size() > other.arr.size();

    std::size_t n = arr.size();
    for (std::size_t i = 0; i != n; i++)
    {
        if (!(arr[i] == other.arr[i])) return arr[i] > other.arr[i];
    }

    return false;
}


bool RightRuleContextFreeGrm::operator>= (const RightRuleContextFreeGrm & other) const
{
    return operator==(other) || operator> (other);
}

bool RightRuleContextFreeGrm::operator< (const RightRuleContextFreeGrm & other) const
{
    return !operator>=(other);
}

bool RightRuleContextFreeGrm::operator!= (const RightRuleContextFreeGrm & other) const
{
    return !operator==(other);
}

bool RightRuleContextFreeGrm::operator<= (const RightRuleContextFreeGrm & other) const
{
    return !operator>(other);
}

std::size_t RightRuleContextFreeGrm::size() const
{
    return arr.size();
}

bool RightRuleContextFreeGrm::empty() const
{
    return arr.empty();
}

std::size_t RightRuleContextFreeGrm::find(const Symbol & sym) const
{
    auto n = arr.size();
    for (decltype(n) i = 0; i != n; i++)
    {
        if (arr[i] == sym) return i;
    }
    return -1;
}

void RightRuleContextFreeGrm::erase(std::size_t pos, std::size_t n)
{
    for (std::size_t i = pos + n; i < arr.size(); i++)
    {
        arr[i - n] = arr[i];
    }

    arr.resize(arr.size() - n);
}

RightRuleContextFreeGrm
RightRuleContextFreeGrm::substr(std::size_t pos, std::size_t n) const
{
    RightRuleContextFreeGrm ret;

    if (n == 0)
    {
        return ret;
    }

    for (std::size_t i = pos; i < pos + n; i++)
    {
        ret.arr.push_back(arr[i]);
    }
    return ret;
}



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
    ProductionContextFreeGrammar();
    void run(grm_t & grm);
private:
    NoChildContextFreeGrammar no_child;
    NoAvailableContextFreeGrammar no_ava;
};

ProductionContextFreeGrammar::ProductionContextFreeGrammar()
: no_child(), no_ava() {}


void NoChildContextFreeGrammar::run(grm_t & grm)
{
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
    for (auto & rule : grm.grm)
    {
        if (is_has_child_str(rule.second))
        {
            cur_N.insert(rule.first);
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

bool NoChildContextFreeGrammar::is_has_child_str(const sequence_t & chain)
{
    return std::all_of(chain.arr.begin(), chain.arr.end(),
                       [this] (Symbol sym) { return is_has_child(sym); } );
}

bool NoChildContextFreeGrammar::is_has_child(Symbol non_terminal)
{
    return non_terminal.kind == TERM || prev_N.find(non_terminal) != prev_N.end();
}

void NoChildContextFreeGrammar::collect_non_terminal(const grm_t & grm)
{
    for (auto & rule : grm.grm)
    {
        non_terminals.insert(rule.first);
        collect_str_non_terminal(rule.second);
    }
}

void
NoChildContextFreeGrammar::collect_str_non_terminal(const sequence_t & str)
{
    for (Symbol sym : str.arr)
    {
        if (sym.kind == NOT_TERM)
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

void NoChildContextFreeGrammar::do_delete(grm_t & grm)
{
    for (Symbol sym : bad_set)
    {
        do_delete_sym(grm, sym);
    }
}

void NoChildContextFreeGrammar::do_delete_sym(grm_t & grm, Symbol sym)
{
    for (auto it = grm.grm.begin(); it != grm.grm.end();)
    {
        if (it->first == sym ||
            std::find(it->second.arr.begin(), it->second.arr.end(), sym) != it->second.arr.end())
        {
            it = grm.grm.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void NoAvailableContextFreeGrammar::run(grm_t & grm)
{
    make_good_set(grm);
    collect_non_terminal(grm);
    make_bad_set();
    do_delete(grm);
}

void NoAvailableContextFreeGrammar::make_good_set(const grm_t & grm)
{
    cur_set.insert(grm.start.begin(), grm.start.end());

    while (!cur_set.empty())
    {
        good_set.insert(cur_set.begin(), cur_set.end());

        for (Symbol sym : cur_set)
        {
            do_step(grm, sym);
        }

        cur_set = temp_set;
        temp_set.clear();
    }
}

void NoAvailableContextFreeGrammar::do_step(const grm_t & grm, Symbol target)
{
    auto range = grm.grm.equal_range(target);

    for (auto it = range.first; it != range.second; it++)
    {
        for (Symbol sym : it->second.arr)
        {
            if (sym.kind == NOT_TERM && good_set.find(sym) == good_set.end())
            {
                temp_set.insert(sym);
            }
        }
    }
}

void NoAvailableContextFreeGrammar::collect_non_terminal(const grm_t & grm)
{
    for (auto & rule : grm.grm)
    {
        non_terminals.insert(rule.first);
        collect_str_non_terminal(rule.second);
    }
}

void
NoAvailableContextFreeGrammar::collect_str_non_terminal(const sequence_t & chain)
{
    for (Symbol sym : chain.arr)
    {
        if (sym.kind == NOT_TERM)
        {
            non_terminals.insert(sym);
        }
    }
}

void NoAvailableContextFreeGrammar::make_bad_set()
{
    std::set_difference(non_terminals.begin(), non_terminals.end(),
                        good_set.begin(), good_set.end(),
                        std::inserter(bad_set, bad_set.begin()));
}

void NoAvailableContextFreeGrammar::do_delete(grm_t & grm)
{
    for (Symbol sym : bad_set)
    {
        do_delete_sym(grm, sym);
    }
}

void NoAvailableContextFreeGrammar::do_delete_sym(grm_t & grm, Symbol sym)
{
    for (auto it = grm.grm.begin(); it != grm.grm.end();)
    {
        if (it->first == sym ||
            std::find(it->second.arr.begin(), it->second.arr.end(), sym) != it->second.arr.end())
        {
            it = grm.grm.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void ProductionContextFreeGrammar::run(grm_t & grm)
{
    no_child.run(grm);
    no_ava.run(grm);
}


class DeleteEmptyRules
{
public:
    DeleteEmptyRules();
    void run(grm_t & grm);
private:
    // step 1
    void build_X(const grm_t & grm);
    // step 2
    void delete_empty_rules(grm_t & grm);
    bool all_in_X(const sequence_t & chain);
    // step 3
    void restore_empty_start(grm_t & grm);
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


DeleteEmptyRules::DeleteEmptyRules() : X() {}


void DeleteEmptyRules::run(grm_t & grm)
{
    build_X(grm);

    delete_empty_rules(grm);

    restore_empty_start(grm);

    update_rules(grm);
}

void DeleteEmptyRules::build_X(const grm_t & grm)
{
    int count = 1;

    while (count > 0)
    {
        count = 0;

        for (const auto & it : grm.grm)
        {
            if (X.find(it.first) == X.end() &&
                ((it.second.size() == 1 && it.second.arr[0].is_empty()) ||
                 all_in_X(it.second)))
            {
                count++;
                X.insert(it.first);
            }
        }
    }
}

void DeleteEmptyRules::delete_empty_rules(grm_t & grm)
{
    for (auto it = grm.grm.begin(); it != grm.grm.end();)
    {
        auto cur = it;
        it++;

        if (cur->second.size() == 1 && cur->second.arr[0].is_empty())
        {
            grm.grm.erase(cur);
        }
    }
}

bool DeleteEmptyRules::all_in_X(const sequence_t & chain)
{
    return std::all_of(chain.arr.begin(), chain.arr.end(),
                       [this] (Symbol sym)
                       {
                           return X.find(sym) != X.end();
                       });
}

void DeleteEmptyRules::update_rules(grm_t & grm)
{
    grm_t new_grm;
    new_grm.start = grm.start;

    for (auto & it : grm.grm)
    {
        add_all_combinations(new_grm, it);
    }

    grm = new_grm;
}

void DeleteEmptyRules::add_all_combinations(grm_t & grm, const pair_t & rule)
{
    std::vector<sequence_t> alpha;
    std::vector<sequence_t> from_X;

    sequence_t right = rule.second;

    alpha.push_back(parse_alpha(right));
    if (right.empty())
    {
        grm.grm.insert(rule);
        return;
    }

    while (!right.empty())
    {
        from_X.push_back(parse_X(right));
        alpha.push_back(parse_alpha(right));
    }

    bool all_empty = std::all_of(alpha.begin(), alpha.end(),
                                 [] (sequence_t & s)
                                 { return s.empty(); });

    int start = all_empty ? 1 : 0;
    int amount = 1 << from_X.size();

    for (int i = start; i < amount; i++)
    {
        grm.grm.emplace(
        rule.first, make_combination(grm, alpha, from_X, i)
        );
    }
}

sequence_t DeleteEmptyRules::parse_alpha(sequence_t & right)
{
    size_t pos = 0, len = right.size();

    while (pos < len && X.find(right.arr[pos]) == X.end())
    {
        pos++;
    }

    sequence_t ret = right.substr(0, pos);
    right.erase(0, pos);
    return ret;
}

sequence_t DeleteEmptyRules::parse_X(sequence_t & right)
{
    sequence_t ret = right.substr(0, 1);
    right.erase(0, 1);
    return ret;
}

sequence_t DeleteEmptyRules::make_combination(
grm_t & grm,
std::vector<sequence_t> & alpha,
std::vector<sequence_t> & from_X,
int mask
)
{
    size_t len = from_X.size();

    sequence_t result;

    for (size_t i = 0; i < len; i++)
    {
        result.arr.insert(result.arr.end(), alpha[i].arr.begin(), alpha[i].arr.end());
        if ((mask & 1) == 1)
        {
            result.arr.insert(result.arr.end(), from_X[i].arr.begin(), from_X[i].arr.end());
        }
        mask >>= 1;
    }

    result.arr.insert(result.arr.end(), alpha[len].arr.begin(), alpha[len].arr.end());
    return result;
}


void DeleteEmptyRules::restore_empty_start(grm_t & grm)
{
    for (auto & sym : grm.start)
    {
        if (X.find(sym) != X.end())
        {
            sequence_t seq;
            seq.arr.emplace_back(Symbol(TERM, 0));
            grm.grm.emplace(sym, seq);
        }
    }
}


using dfa_t = std::multimap<sequence_t, std::pair<sequence_t, Symbol>>;
using dfa_rule_t = std::pair<sequence_t, Symbol>;


class LeftDFAMgr
{
public:
    LeftDFAMgr();

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

LeftDFAMgr::LeftDFAMgr() :
result(), dfa(), watched(), new_names(), que(), terminals() {}



void LeftDFAMgr::build_dfa(const grm_t & grm)
{
    collect_terminals(grm);

    sequence_t seq;
    seq.arr.push_back(start);
    que.insert(seq);

    while (!que.empty())
    {
        do_step(grm);
    }

    get_new_names();
    convert_dfa_to_grm();
    collect_finite_not_terminals(grm);
}


void LeftDFAMgr::do_step(const grm_t & grm)
{
    sequence_t current = *que.begin();
    que.erase(current);

    for (Symbol terminal : terminals)
    {
        sequence_t value = get_all_values(grm, current, terminal);

        if (!value.empty())
        {
            dfa.emplace(value, dfa_rule_t(current, terminal));

            if (watched.find(value) == watched.end() && value != current)
            {
                que.insert(value);
            }
        }
    }

    watched.insert(current);
}

sequence_t LeftDFAMgr::get_all_values(const grm_t & grm,
                                      const sequence_t & vertex,
                                      Symbol terminal)
{
    std::set<Symbol> values;

    for (auto & sym : vertex.arr)
    {
        std::set<Symbol> cur = get_non_terminal_all_values(grm, sym, terminal);
        values.insert(cur.begin(), cur.end());
    }

    sequence_t ret;
    for (auto & sym : values)
    {
        ret.arr.push_back(sym);
    }
    return ret;
}

std::set<Symbol>
LeftDFAMgr::get_non_terminal_all_values(const grm_t & grm,
                                        Symbol non_terminal,
                                        Symbol terminal)
{
    std::set<Symbol> ret;

    for (auto & [nt, value] : grm.grm)
    {
        if ((value.size() == 1 && non_terminal == start && value.arr[0] == terminal) ||
            (value.size() == 2 && value.arr[0] == non_terminal && value.arr[1] == terminal))
        {
            ret.insert(nt);
        }
    }

    return ret;
}

void LeftDFAMgr::collect_terminals(const grm_t & grm)
{
    for (auto & rule : grm.grm)
    {
        for (auto & sym : rule.second.arr)
        {
            if (sym.kind == TERM)
            {
                terminals.insert(sym);
            }
        }
    }
}

void LeftDFAMgr::convert_dfa_to_grm()
{
    for (auto & [left, right] : dfa)
    {
        sequence_t temp;
        if (!(right.first.size() == 1 && right.first.arr[0].is_H()))
        {
            temp.arr.push_back(new_names[right.first]);
        }
        temp.arr.push_back(right.second);

        result.grm.emplace(new_names[left], temp);
    }
}

void LeftDFAMgr::get_new_names()
{
    for (auto & nt : watched)
    {
        if (nt.arr.size() == 1)
        {
            new_names[nt] = nt.arr[0];
        }
        else
        {
            new_names[nt] = Symbol(NOT_TERM, Symbol::next_name());
        }
    }
}

void LeftDFAMgr::collect_finite_not_terminals(const grm_t & grm)
{
    for (auto & name : watched)
    {
        std::set<Symbol> sims(name.arr.begin(), name.arr.end());
        std::vector<Symbol> temp;
        std::set_intersection(sims.begin(), sims.end(),
                              grm.start.begin(), grm.start.end(),
                              std::inserter(temp, temp.begin()));
        if (!temp.empty())
        {
            result.start.insert(new_names[name]);
        }
    }
}


class RegexChecker
{
public:
    RegexChecker(const std::string & re);

    bool check(const std::string & str);

    void exec_digit();
    void exec_char(char repr);
    void exec_empty();
    void exec_concat();
    void exec_or();
    void exec_iter_plus();
    void exec_iter_star();

private:
    bool check_inside(Symbol cur_state, const std::string & str);

    static void process_grm(grm_t & grm);

    void build_dfa();

    static bool is_empty(const grm_t & grm);
    static bool is_empty_chain(const grm_t & grm);

    static void add_empty(grm_t & grm);

    grm_t grm_;
    usefull_dfa_t dfa_;

    RegexParser parser_;
    std::vector<grm_t> stack_;
    static const int digit_repr;
};


void RegexChecker::add_empty(grm_t & grm)
{
    if (is_empty(grm))
    {
        Symbol sym = Symbol(NOT_TERM, Symbol::next_name());
        grm.start.insert(sym);
        sequence_t seq;
        seq.arr.emplace_back(TERM, 0);
        grm.grm.emplace(sym, seq);
    }
}


RegexChecker::RegexChecker(const std::string & re) :
grm_(), dfa_(), parser_(), stack_()
{
    std::stringstream stream;
    stream << re;

    try {
        parser_.parse(stream);
    }
    catch (...)
    {
        throw std::runtime_error("INCORRECT REGEX");
    }

    int star_cnt = 0, plus_cnt = 0;

    for (auto it : parser_)
    {
        switch (it.kind)
        {
            case PolizItem::Kind::CHAR:
                star_cnt = plus_cnt = 0;
                exec_char(it.repr);
                break;
            case PolizItem::Kind::DIGIT:
                star_cnt = plus_cnt = 0;
                exec_digit();
                break;
            case PolizItem::Kind::CONCAT:
                star_cnt = plus_cnt = 0;
                exec_concat();
                break;
            case PolizItem::Kind::OR:
                star_cnt = plus_cnt = 0;
                exec_or();
                break;
            case PolizItem::Kind::ITER_STAR:
                if (star_cnt == 0)
                {
                    exec_iter_star();
                }
                star_cnt++;
                break;
            case PolizItem::Kind::ITER_PLUS:
                if (star_cnt == 0 && plus_cnt == 0)
                {
                    exec_iter_plus();
                }
                plus_cnt++;
                break;
            case PolizItem::Kind::EMPTY:
                star_cnt = plus_cnt = 0;
                exec_empty();
                break;
            default:
                star_cnt = plus_cnt = 0;
                break;
        }
    }

    grm_ = stack_[0];
    stack_.pop_back();

    if (is_empty(grm_))
    {
        Symbol sym = Symbol(NOT_TERM, Symbol::next_name());
        grm_.start.insert(sym);
        sequence_t seq;
        seq.arr.emplace_back(TERM, 0);
        grm_.grm.emplace(sym, seq);
    }

    build_dfa();
}


bool RegexChecker::check(const std::string & str)
{
    std::string rev = str;
    std::reverse(rev.begin(), rev.end());

    return check_inside(Symbol(NOT_TERM, 0), rev);
}


bool RegexChecker::check_inside(Symbol cur_state, const std::string & str)
{
    if (str.empty() && grm_.start.find(cur_state) != grm_.start.end())
    {
        return true;
    }

    auto & paths = dfa_[cur_state];

    for (auto it : paths)
    {
        if (!str.empty() &&
            (it.first.repr == str[0] ||
             (it.first.repr == digit_repr && std::isdigit(str[0]))))
        {
            if (check_inside(it.second, str.substr(1)))
            {
                return true;
            }
        }
        else if (it.first.repr == 0)
        {
            if (check_inside(it.second, str.substr(0)))
            {
                return true;
            }
        }
    }

    return false;
}


void RegexChecker::exec_char(char repr)
{
    grm_t grm;
    Symbol sym = Symbol(NOT_TERM, Symbol::next_name());
    grm.start.insert(sym);
    sequence_t seq;
    seq.arr.emplace_back(TERM, repr);
    grm.grm.emplace(sym, seq);
    stack_.push_back(grm);
}


void RegexChecker::exec_digit()
{
    grm_t grm;
    Symbol sym = Symbol(NOT_TERM, Symbol::next_name());
    grm.start.insert(sym);
    sequence_t seq;
    seq.arr.emplace_back(TERM, digit_repr);
    grm.grm.emplace(sym, seq);
    stack_.push_back(grm);
}


void RegexChecker::exec_empty()
{
    stack_.emplace_back();
}


void RegexChecker::exec_or()
{
    if (stack_.size() < 2) throw std::exception();

    auto a = stack_[stack_.size() - 2], b = stack_[stack_.size() - 1];

    if (is_empty(a))
    {
        if (is_empty_chain(b))
        {
            stack_.push_back(b);
            return;
        }
    }

    if (is_empty(b))
    {
        if (is_empty_chain(a))
        {
            stack_.push_back(a);
            return;
        }
    }

    if (is_empty(a) && is_empty(b))
    {
        stack_.push_back(a);
        return;
    }

    add_empty(a);
    add_empty(b);

    grm_t res;

    res.start.insert(a.start.begin(), a.start.end());
    res.start.insert(b.start.begin(), b.start.end());

    res.grm.insert(a.grm.begin(), a.grm.end());
    res.grm.insert(b.grm.begin(), b.grm.end());

    stack_.pop_back();
    stack_.pop_back();

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::exec_concat()
{
    if (stack_.size() < 2) throw std::exception();

    grm_t b = stack_.back();
    stack_.pop_back();
    grm_t a = stack_.back();
    stack_.pop_back();

    grm_t res;

    if (is_empty(a))
    {
        stack_.push_back(b);
        return;
    }

    if (is_empty(b))
    {
        stack_.push_back(a);
        return;
    }

    res.start.insert(a.start.begin(), a.start.end());

    for (auto it = a.grm.begin(); it != a.grm.end(); )
    {
        if (it->second.size() == 1)
        {
            for (auto & finite : b.start)
            {
                sequence_t seq;
                seq.arr.emplace_back(finite);
                seq.arr.emplace_back(it->second.arr[0]);

                res.grm.emplace(it->first, seq);
            }

            it = a.grm.erase(it);
        }
        else
        {
            ++it;
        }
    }

    res.grm.insert(a.grm.begin(), a.grm.end());
    res.grm.insert(b.grm.begin(), b.grm.end());

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::exec_iter_plus()
{
    if (stack_.empty()) throw std::exception();

    grm_t a = stack_.back();
    stack_.pop_back();

    if (is_empty(a))
    {
        stack_.push_back(a);
        return;
    }

    grm_t res = a;

    for (auto & it : a.grm)
    {
        if (it.second.size() == 1)
        {
            for (auto & finite : a.start)
            {
                sequence_t seq;
                seq.arr.emplace_back(finite);
                seq.arr.emplace_back(it.second.arr[0]);

                res.grm.emplace(it.first, seq);
            }
        }
    }

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::exec_iter_star()
{
    if (stack_.empty()) throw std::exception();

    grm_t a = stack_.back();
    stack_.pop_back();

    if (is_empty(a))
    {
        stack_.push_back(a);
        return;
    }

    grm_t res = a;

    for (auto & it : a.grm)
    {
        if (it.second.size() == 1)
        {
            for (auto & finite : a.start)
            {
                sequence_t seq;
                seq.arr.emplace_back(finite);
                seq.arr.emplace_back(it.second.arr[0]);

                res.grm.emplace(it.first, seq);
            }
        }
    }

    sequence_t seq;
    seq.arr.emplace_back(TERM, 0);

    for (auto & it : res.start)
    {
        res.grm.emplace(it, seq);
    }

    process_grm(res);
    stack_.push_back(res);
}


void RegexChecker::process_grm(grm_t & grm)
{
    LeftDFAMgr mgr;
    mgr.build_dfa(grm);
    grm = mgr.result;

    DeleteEmptyRules del;
    del.run(grm);

    ProductionContextFreeGrammar prod;
    prod.run(grm);
}


void RegexChecker::build_dfa()
{

    for (auto & it : grm_.grm)
    {
        if (it.second.size() == 1)
        {
            dfa_[Symbol(NOT_TERM, 0)][it.second.arr[0]] = it.first;
        }
        else
        {
            dfa_[it.second.arr[0]][it.second.arr[1]] = it.first;
        }
    }
}


const int RegexChecker::digit_repr = 10000;


bool RegexChecker::is_empty(const grm_t & grm)
{
    return grm.grm.empty();
}


bool RegexChecker::is_empty_chain(const grm_t & grm)
{
    for (auto finite : grm.start)
    {
        auto range = grm.grm.equal_range(finite);

        for (auto it = range.first; it != range.second; it++)
        {
            if (it->second.size() == 1 && it->second.arr[0].is_empty())
            {
                return true;
            }
        }
    }
    return false;
}



int main()
{
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