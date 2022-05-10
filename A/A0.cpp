/*
 * S -> aabS
 * S -> aaSb
 * S -> aSab
 * S -> abaS
 * S -> abSa
 * S -> aSba
 * S -> baaS
 * S -> baSa
 * S -> bSaa
 * S -> aab
 * S -> aba
 * S -> baa
 * S -> EMPTY
 */


#include <iostream>


class Checker
{
public:
    Checker();
    bool check(const std::string & str);
private:
    void S();

    bool is_context_ok() const;

    void match(char sym);
    void next();

    void reset();

    int count;
    bool is_end;
    size_t pos;
    char cur;
    const std::string * str_;
};


Checker::Checker() :
count(0), is_end(true), pos(0), cur('\0'), str_(nullptr) {}


bool Checker::check(const std::string & str)
{
    reset();
    is_end = false;
    str_ = &str;

    try {
        next();
        S();
        if (!is_end) throw std::exception();
    }
    catch (std::exception &)
    {
        reset();
        return false;
    }

    bool ret = is_context_ok();
    reset();
    return ret;
}


void Checker::S()
{
    if (is_end)
    {
        // S -> EMPTY
    }
    else if (cur == 'a')
    {
        /*<*/ count -= 1; /*>*/

        match('a');
        S();
    }
    else if (cur == 'b')
    {
        /*<*/ count += 2; /*>*/

        match('b');
        S();
    }
    else
    {
        throw std::exception();
    }
}


void Checker::reset()
{
    str_ = nullptr;
    pos = 0;
    cur = '\0';
    count = 0;
    is_end = true;
}


void Checker::match(char sym)
{
    if (is_end || cur != sym)
    {
        throw std::exception();
    }
    next();
}


void Checker::next()
{
    if (str_ != nullptr && !is_end)
    {
        if (pos >= str_->size())
        {
            is_end = true;
        }
        else
        {
            cur = (*str_)[pos++];
        }
    }
    else
    {
        is_end = true;
    }
}


bool Checker::is_context_ok() const
{
    return count == 0;
}


int main()
{
    std::cout << 2 << std::endl;

    Checker checker;

    std::string str;
    while (getline(std::cin, str))
    {
        if (checker.check(str))
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
