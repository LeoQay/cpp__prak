#ifndef CPP__PRAK_GLOBAL_TYPEDEF_H
#define CPP__PRAK_GLOBAL_TYPEDEF_H


#include <map>
#include <set>
#include <vector>



/*
 * If kind = NOT_TERMINAL, repr = 0 means 'H',
 * if kind = TERMINAL, repr = 0, means imaginary finite terminal
 */

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


struct ContextFreeGrm
{
    std::set<Symbol> start;
    std::multimap<Symbol, RightRuleContextFreeGrm> grm;
};


using sequence_t = RightRuleContextFreeGrm;
using grm_t = ContextFreeGrm;
using pair_t = std::pair<Symbol, RightRuleContextFreeGrm>;


#endif //CPP__PRAK_GLOBAL_TYPEDEF_H
