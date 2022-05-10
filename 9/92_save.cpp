#include <iostream>
#include <string>
#include <map>
#include <unordered_set>
#include <unordered_map>


typedef std::multimap<char, std::string> grm_t;
typedef std::pair<char, std::string> rule_t;
typedef std::unordered_set<char> char_set;
typedef std::unordered_set<std::string> str_set;
typedef std::unordered_map<
std::string,
std::pair<bool, std::unordered_set<char>>
> stat_t;

class GrmValidityRDM
{
public:
    GrmValidityRDM();
    bool check(const grm_t & _grm);
private:
    void build_first();
    void reset_first();
    void build_first_chain(const std::string & chain, char_set & cont);
    void build_first_non_terminal(char sym, char_set & cont);
    bool is_determined(const std::string & str);

    void build_follow();

    void build_to_empty();
    bool is_empty_str(const std::string & str);

    void build_non_terminals();

    static bool is_non_terminal(char sym);

    static std::string to_str(char sym);

    const grm_t * grm;
    stat_t first_stat;
    stat_t follow_stat;
    char_set to_empty;
    char_set non_terminals;
    str_set pool;
};

GrmValidityRDM::GrmValidityRDM() :
grm(nullptr), first_stat(), follow_stat(),
to_empty(), non_terminals() {}


bool GrmValidityRDM::check(const grm_t & _grm)
{
    grm = &_grm;

    build_to_empty();
    build_non_terminals();

    build_first();

    grm = nullptr;
}


void GrmValidityRDM::build_first()
{
    reset_first();

    for (auto & [l, r] : *grm)
    {
        pool.clear();
        auto & [flag, stat] = first_stat[r];
        build_first_chain(r, stat);
    }
}

void GrmValidityRDM::build_first_non_terminal(char sym, char_set & cont)
{
    auto str = to_str(sym);

    pool.insert(str);

    auto & [cur_f, cur_stat] = first_stat[str];

    if (cur_f)
    {
        cont.insert(cur_stat.begin(), cur_stat.end());
        pool.erase(str);
        return;
    }

    auto range = grm->equal_range(sym);

    for (auto it = range.first; it != range.second; it++)
    {
        if (pool.find(it->second) != pool.end())
        {
            continue;
        }

        auto & [flag, stat] = first_stat[it->second];

        if (flag)
        {
            cont.insert(stat.begin(), stat.end());
        }
        else
        {
            build_first_chain(it->second, cont);
        }
    }

    pool.erase(str);
}

void GrmValidityRDM::build_first_chain(const std::string & chain, char_set & cont)
{
    auto & [cur_f, cur_stat] = first_stat[chain];

    if (cur_f)
    {
        cont.insert(cur_stat.begin(), cur_stat.end());
        return;
    }

    size_t pos = 0;
    size_t len = chain.length();

    while (true)
    {
        if (pos >= len)
        {
            break;
        }

        char sym = chain[pos];
        auto str = to_str(sym);

        if (!is_non_terminal(sym))
        {
            cont.insert(sym);
            break;
        }

        auto & [flag, stat] = first_stat[str];

        if (flag)
        {
            cont.insert(stat.begin(), stat.end());
            break;
        }

        build_first_non_terminal(sym, cont);

        if (to_empty.find(sym) == to_empty.end())
        {
            break;
        }

        pos++;
    }
}


bool GrmValidityRDM::is_determined(const std::string & str)
{
    return std::all_of(str.begin(), str.end(),
                       [this] (char sym)
                       {
                           return !is_non_terminal(sym) || first_stat[to_str(sym)].first;
                       });
}

void GrmValidityRDM::reset_first()
{
    first_stat.clear();

    for (auto & [l, r] : *grm)
    {
        //first_stat[to_str(l)] = {false, {}};
        first_stat[r] = {false, {}};
    }
}

bool GrmValidityRDM::is_non_terminal(char sym)
{
    return sym >= 'A' && sym <= 'Z';
}


void GrmValidityRDM::build_to_empty()
{
    to_empty.clear();

    size_t prev_size, cur_size = 0;

    while (cur_size != prev_size)
    {
        prev_size = cur_size;

        for (auto & [l, r] : *grm)
        {
            if (is_empty_str(r))
            {
                to_empty.insert(l);
            }
        }

        cur_size = to_empty.size();
    }
}

bool GrmValidityRDM::is_empty_str(const std::string & str)
{
    return str.empty() || std::all_of(str.begin(), str.end(),
                                      [this] (char sym)
                                      {return to_empty.find(sym) != to_empty.end();});
}

std::string GrmValidityRDM::to_str(char sym)
{
    std::string s = "_";
    s[0] = sym;
    return s;
}

void GrmValidityRDM::build_non_terminals()
{
    non_terminals.clear();

    for (auto & [l, r] : *grm)
    {
        non_terminals.insert(l);

    }
}



/*
S:aA
S:BDc
A:BAa
A:aB
A:b
B:
D:B
D:b
end
 */

/*
S:BDc
B:
D:B
D:b
end
 */


int main()
{
    grm_t grm;
    std::string str;
    while (getline(std::cin, str))
    {
        if (str == "end")
        {
            break;
        }

        auto pos = str.find(':');
        grm.emplace(str[0], str.substr(pos + 1));
    }

    GrmValidityRDM checker;

    if (checker.check(grm))
    {
        std::cout << "YES" << std::endl;
    }
    else
    {
        std::cout << "NO" << std::endl;
    }

    return 0;
}