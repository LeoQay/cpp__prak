#include <iostream>
#include <algorithm>
#include <string>
#include <map>


typedef std::pair<std::string, std::string> rule_t;


bool check_sym_not_terminal(char sym)
{
    return sym >= 'A' && sym <= 'Z';
}

bool check_2(const std::multimap<std::string, std::string> & g)
{
    return std::all_of(g.begin(), g.end(),
                       [] (const rule_t & rule)
                       { return rule.first.size() == 1 &&
                       check_sym_not_terminal(rule.first[0]); } );
}

bool check_str_not_terminal(const std::string & str)
{
    return std::any_of(str.begin(), str.end(),
                       [] (char sym) { return check_sym_not_terminal(sym); } );
}

bool check_right_side(const std::multimap<std::string, std::string> & g)
{
    for (auto & rule : g)
    {
        if (rule.second.empty())
        {
            continue;
        }

        size_t start = 0;
        if (check_sym_not_terminal(rule.second[0]))
        {
            start = 1;
        }

        if (check_str_not_terminal(rule.second.substr(start)))
        {
            return false;
        }
    }

    return true;
}

bool check_left_side(const std::multimap<std::string, std::string> & g)
{
    for (auto & rule : g)
    {
        if (rule.second.empty())
        {
            continue;
        }

        size_t len = rule.second.size();
        if (check_sym_not_terminal(rule.second[len - 1]))
        {
            len--;
        }

        if (check_str_not_terminal(rule.second.substr(0, len)))
        {
            return false;
        }
    }

    return true;
}

bool check_3(const std::multimap<std::string, std::string> & g)
{
    if (!check_2(g))
    {
        return false;
    }

    return check_right_side(g) || check_left_side(g);
}

bool check_1(const std::multimap<std::string, std::string> & g)
{
    bool is_s_empty = false;
    bool is_s_right = false;

    for (auto & rule : g)
    {
        if (rule.first == "S" && rule.second.empty())
        {
            is_s_empty = true;
            continue;
        }

        if (static_cast<int>(rule.second.find('S')) != -1)
        {
            is_s_right = true;
        }

        if (rule.first.size() > rule.second.size())
        {
            return false;
        }
    }

    if (is_s_empty && is_s_right)
    {
        return false;
    }

    return true;
}

bool check_0(const std::multimap<std::string, std::string> & g)
{
    return std::all_of(g.begin(), g.end(),
                       [] (const rule_t & rule)
                       {
        return std::any_of(rule.first.begin(), rule.first.end(),
                           [] (char sym)
                           {
            return check_sym_not_terminal(sym);
                           });
                       });
}



int grammar_type(const std::multimap<std::string, std::string> & g)
{
    if (!check_0(g))
    {
        return -1;
    }

    if (check_3(g))
    {
        return 3;
    }

    if (check_2(g))
    {
        return 2;
    }

    if (check_1(g))
    {
        return 1;
    }

    return 0;
}


/*
void test()
{
    std::multimap<std::string, std::string> grm;

    grm.insert(rule_t("S", "2"));


    std::cout << grammar_type(grm) << std::endl;
}
*/



int main()
{
    //test();

    std::multimap<std::string, std::string> grm;

    grm.insert(rule_t("S", "A"));
    grm.insert(rule_t("S", "aBCd"));
    grm.insert(rule_t("BC", "bc"));

    int type = grammar_type(grm);

    std::cout << type << std::endl;

    return 0;
}
