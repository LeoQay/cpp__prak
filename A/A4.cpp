#include <iostream>
#include <string>



struct Stat
{
    Stat();

    void reset();

    int closed;
    int count_comp;
    int count_or;
    int count_and;
    int count_plus;
    int count_mul;
    int arithmetic_operands;
    int logic_operands;
};

Stat::Stat() :
closed(0), count_comp(0), count_or(0), count_and(0),
count_plus(0), count_mul(0), arithmetic_operands(0),
logic_operands(0) {}

void Stat::reset()
{
    closed = 0;
    count_comp = 0;
    count_or = 0;
    count_and = 0;
    count_plus = 0;
    count_mul = 0;
    arithmetic_operands = 0;
    logic_operands = 0;
}



class Parser
{
public:
    Parser();
    Parser(const Parser & obj) = delete;
    Parser & operator= (const Parser & obj) = delete;
    bool parse(std::string & str);
private:

    void expression();
    void disjunction();
    void disjunction_continue();
    void conjunction();
    void conjunction_continue();
    void arithmetic();
    void arithmetic_continue();
    void term();
    void term_continue();
    void op();
    void comp_continue();

    static void assert_one_type(const Stat * obj);
    static void assert_true(bool logic);
    static bool is_comp(char sym);
    static bool is_arithmetic(const Stat * obj);
    static bool is_logic(const Stat * obj);

    void match(char sym);
    void next();
    void reset();

    bool is_end;
    char cur;
    size_t pos;
    const std::string * str_;

    Stat stat;
    Stat * cur_stat;
};


Parser::Parser() :
is_end(true), cur('\0'), pos(0), str_(nullptr),
stat(), cur_stat(nullptr) {}


bool Parser::parse(std::string & str)
{
    reset();
    is_end = false;
    str_ = &str;

    try {
        next();
        expression();
        if (!is_end) throw std::exception();
    }
    catch (std::exception &)
    {
        reset();
        return false;
    }

    reset();
    return true;
}


void Parser::expression()
{
    /*<*/
    Stat * prev = cur_stat;
    Stat buf;
    cur_stat = &buf;
    /*>*/

    disjunction();

    /*<*/
    assert_one_type(&buf);
    /*>*/
}


void Parser::disjunction()
{
    conjunction();
    disjunction_continue();
}


void Parser::disjunction_continue()
{
    if (is_end || cur != '|')
    {
        // contD -> EMPTY

        /*<*/
        if (cur_stat->count_or > 0 || is_logic(cur_stat))
        {
            cur_stat->logic_operands++;
        }
        else
        {
            cur_stat->arithmetic_operands++;
        }
        /*>*/
    }
    else
    {
        /*<*/
        cur_stat->count_or++;
        cur_stat->logic_operands++;
        /*>*/

        match('|');
        conjunction();
    }
}


void Parser::conjunction()
{
    arithmetic();
    conjunction_continue();
}


void Parser::conjunction_continue()
{
    if (is_end || cur != '&')
    {
        // contK -> EMPTY

        /*<*/
        if (cur_stat->count_and > 0 || is_logic(cur_stat))
        {
            cur_stat->logic_operands++;
        }
        else
        {
            cur_stat->arithmetic_operands++;
        }
        /*>*/
    }
    else
    {
        /*<*/
        cur_stat->count_and++;
        cur_stat->logic_operands++;
        /*>*/

        match('&');
        arithmetic();
    }
}


void Parser::arithmetic()
{
    term();
    arithmetic_continue();
}


void Parser::arithmetic_continue()
{
    if (is_end || cur != '+')
    {
        // contA -> EMPTY

        /*<*/
        if (cur_stat->count_plus > 0 || is_arithmetic(cur_stat))
        {
            cur_stat->arithmetic_operands++;
        }
        else
        {
            cur_stat->logic_operands++;
        }
        /*>*/
    }
    else
    {
        /*<*/
        cur_stat->count_plus++;
        cur_stat->arithmetic_operands++;
        /*>*/

        match('+');
        arithmetic();
    }
}


void Parser::term()
{
    op();
    term_continue();
}


void Parser::term_continue()
{
    if (is_end || cur != '*')
    {
        // contT -> EMPTY

        /*<*/
        if (cur_stat->count_mul > 0 || is_arithmetic(cur_stat))
        {
            cur_stat->arithmetic_operands++;
        }
        else
        {
            cur_stat->logic_operands++;
        }
        /*>*/
    }
    else
    {
        /*<*/
        cur_stat->arithmetic_operands++;
        cur_stat->count_mul++;
        /*>*/

        match('*');
        term();
    }
}


void Parser::op()
{
    if (is_end) throw std::exception();

    if (std::isdigit(cur))
    {
        /*<*/
        cur_stat->arithmetic_operands++;
        /*>*/

        match(cur);
    }
    else if (cur == 'T' || cur == 'F')
    {
        /*<*/
        cur_stat->logic_operands++;
        /*>*/

        match(cur);
    }
    else if (cur == '(')
    {

        match('(');
        expression();
        match(')');
    }
    else
    {
        throw std::exception();
    }
}


void Parser::comp_continue()
{
    if (is_end || !is_comp(cur))
    {
        // contE -> EMPTY
    }
    else
    {
        /*<*/
        cur_stat->count_comp++;
        /*>*/

        match(cur);

        /*<*/
        Stat * prev = cur_stat;
        Stat buf;
        cur_stat = &buf;
        /*>*/

        arithmetic();

        /*<*/
        cur_stat = prev;
        /*>*/
    }
}





void Parser::assert_one_type(const Stat * obj)
{
    bool ar = is_arithmetic(obj);
    bool log = is_logic(obj);
    assert_true(ar && !log || !ar && log);
}

void Parser::assert_true(bool logic)
{
    if (!logic)
    {
        throw std::exception();
    }
}

bool Parser::is_comp(char sym)
{
    return sym == '>' || sym == '<' || sym == '=';
}

bool Parser::is_arithmetic(const Stat * obj)
{
    return obj->count_and == 0 && obj->count_or == 0 &&
    obj->count_comp == 0 && obj->logic_operands == 0;
}

bool Parser::is_logic(const Stat * obj)
{
    return obj->count_comp >= 0 && obj->count_comp <= 1 &&
    obj->count_plus == 0 && obj->count_mul == 0 &&
    obj->arithmetic_operands == 0;
}


void Parser::reset()
{
    str_ = nullptr;
    pos = 0;
    is_end = true;
    cur = '\0';

    stat.reset();
    cur_stat = &stat;
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
            std::cout << "YES" << std::endl;
        }
        else
        {
            std::cout << "NO" << std::endl;
        }
    }

    return 0;
}

