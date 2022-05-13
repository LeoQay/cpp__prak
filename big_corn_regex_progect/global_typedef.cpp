#include "global_typedef.h"


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
    for (std::size_t i = pos + n; i < arr.size(); i++)
    {
        ret.arr[i - n - pos] = arr[i];
    }
    return ret;
}

