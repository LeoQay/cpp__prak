#include <iostream>
#include <string>


class Parser
{
public:
    Parser();
    Parser(const Parser & obj) = delete;
    Parser & operator= (const Parser & obj) = delete;
    bool parse(std::string & str);
private:

    // S -> aT | bT
    // T -> aT | bT | eps
    void S();
    void T();

    void match(char sym);
    void next();

    void reset();

    bool is_end;
    char cur;
    size_t pos;
    const std::string * str_;

    std::string two;
    std::string reverse;
};


Parser::Parser() :
is_end(true), cur('\0'), pos(0), str_(nullptr),
two(), reverse() {}


bool Parser::parse(std::string & str)
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

    str = two + reverse;
    reset();
    return true;
}


void Parser::S()
{
    if (is_end) throw std::exception();

    if (cur == 'a')
    {
        /*<*/
        reverse.insert(0, 1, 'a');
        two += '2';
        /*>*/

        match('a');
        T();
    }
    else if (cur == 'b')
    {
        /*<*/
        reverse.insert(0, 1, 'b');
        /*>*/

        match('b');
        T();
    }
    else
    {
        throw std::exception();
    }
}


void Parser::T()
{
    if (is_end || (cur != 'a' && cur != 'b'))
    {
        // T -> eps
    }
    else if (cur == 'a')
    {
        /*<*/
        reverse.insert(0, 1, 'a');
        two += '2';
        /*>*/

        match(cur);
        T();
    }
    else
    {
        /*<*/
        reverse.insert(0, 1, 'b');
        /*>*/

        match('b');
        T();
    }
}


void Parser::reset()
{
    str_ = nullptr;
    pos = 0;
    is_end = true;
    cur = '\0';
    two.clear();
    reverse.clear();
}


void Parser::match(char sym)
{
    if (is_end || cur != sym)
    {
        throw std::exception();
    }
    next();
}


void Parser::next()
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




int main()
{
    Parser parser;

    std::string str;
    while (getline(std::cin, str))
    {
        if (parser.parse(str))
        {
            std::cout << str << std::endl;
        }
        else
        {
            std::cout << "NO" << std::endl;
        }
    }

    return 0;
}

