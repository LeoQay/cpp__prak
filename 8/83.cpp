#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>

typedef std::multimap<std::string, std::string> grm_t;
typedef std::multimap<std::string, std::pair<std::string, char>> dfa_t;
typedef std::pair<std::string, char> dfa_rule_t;
typedef std::unordered_map<std::string, std::unordered_map<char, std::string>>
dfa_ds_t;

/*
grm_t g()
{
    return
    {
        {"S", "Aa"},
        {"S", "a"},
        {"A", "Aa"},
        {"A", "Ab"},
        {"A", "a"},
        {"A", "b"}
    };
}
 */


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



int main()
{
    grm_t grm = {};

    LeftDFAMgr mgr(grm);

    std::string str;
    while (getline(std::cin, str))
    {
        if (mgr.check(str))
        {
            std::cout << "YES" << std::endl;
        }
        else
        {
            std::cout << "NO" << std::endl;
        }
    }

    return 0;
}



LeftDFAMgr::LeftDFAMgr(const grm_t & grm)
: dfa(), ds(), watched(), que(), finite(), terminals()
{
    collect_terminals(grm);

    que.insert(start_sym);

    while (!que.empty())
    {
        do_step(grm);
    }

    convert_dfa_to_ds();
}

void LeftDFAMgr::do_step(const grm_t & grm)
{
    std::string current = *que.begin();
    que.erase(current);

    for (char terminal : terminals)
    {
        std::string value = get_all_values(grm, current, terminal);

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

    if (current.find('S') < current.size())
    {
        finite.insert(current);
    }
}

void LeftDFAMgr::collect_terminals(const grm_t & grm)
{
    for (auto & rule : grm)
    {
        for (char sym : rule.second)
        {
            if (!(sym >= 'A' && sym <= 'Z'))
            {
                terminals.insert(sym);
            }
        }
    }
}

std::string
LeftDFAMgr::get_all_values(const grm_t & grm,
                           const std::string & vertex,
                           char terminal)
{
    std::set<char> values;

    for (char sym : vertex)
    {
        std::set<char> cur = get_non_terminal_all_values(grm, sym, terminal);
        values.insert(cur.begin(), cur.end());
    }

    std::string ret;
    for (char sym : values)
    {
        ret += sym;
    }
    return ret;
}

std::set<char>
LeftDFAMgr::get_non_terminal_all_values(const grm_t & grm,
                                        char non_terminal,
                                        char terminal)
{
    std::set<char> ret;

    for (auto [nt, value] : grm)
    {
        if ((value.size() == 1 && non_terminal == 'H' && value[0] == terminal) ||
        (value.size() == 2 && value[0] == non_terminal && value[1] == terminal))
        {
            ret.insert(nt[0]);
        }
    }

    return ret;
}

void LeftDFAMgr::convert_dfa_to_ds()
{
    for (auto & [dest, src] : dfa)
    {
        ds[src.first][src.second] = dest;
    }
}

bool LeftDFAMgr::check(const std::string & str)
{
    std::string current_state = "H";
    std::string temp = str;

    if (terminals.find('_') != terminals.end())
    {
        temp += '_';
    }

    while (true)
    {
        if (temp.empty())
        {
            return finite.find(current_state) != finite.end();
        }

        char cur_sym = temp[0];
        temp.erase(0, 1);

        auto & edges = ds[current_state];

        if (edges.find(cur_sym) == edges.end())
        {
            return false;
        }

        current_state = edges[cur_sym];
    }
}
