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

    void expression();
    void expression_continue();
    void term();
    void term_continue();
    void mul();
    void closed_expression();

    void match(char sym);
    void next();
    void reset();

    bool is_end;
    char cur;
    size_t pos;
    const std::string * str_;

    int * local_mul_counter;
    int global_term_counter;
    int scope_flag;
    std::string * write;
    std::string result;
};


Parser::Parser() :
is_end(true), cur('\0'), pos(0), str_(nullptr),
local_mul_counter(nullptr), global_term_counter(0),
scope_flag(0), write(nullptr), result() {}


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

    str = result;
    reset();
    return true;
}


void Parser::expression()
{
    /*<*/
    int * prev_mul_counter = local_mul_counter;
    int mul_counter = 0;
    local_mul_counter = &mul_counter;
    /*>*/

    term();

    /*<*/
    local_mul_counter = prev_mul_counter;
    global_term_counter++;
    /*>*/

    expression_continue();
}


void Parser::expression_continue()
{
    if (is_end || cur != '+')
    {
        // subE -> EMPTY
    }
    else
    {
        /*<*/
        write->push_back('+');
        /*>*/

        match('+');
        expression();
    }
}


void Parser::term()
{
    /*<*/
    std::string mul_buffer;
    std::string * prev_ptr = write;
    write = &mul_buffer;
    /*>*/

    mul();

    /*<*/
    (*local_mul_counter)++;
    /*>*/

    term_continue();

    /*<*/
    prev_ptr->append(*write);
    write = prev_ptr;
    /*>*/
}


void Parser::term_continue()
{
    if (is_end || cur != '*')
    {
        // subT -> EMPTY

        /*<*/
        if (scope_flag == 1 && *local_mul_counter >= 2)
        {
            write->insert(0, "(");
            write->push_back(')');
        }
        scope_flag = 0;
        /*>*/
    }
    else
    {
        /*<*/
        if (scope_flag == 1)
        {
            write->insert(0, "(");
            write->push_back(')');
        }
        scope_flag = 0;

        write->push_back('*');
        /*>*/

        match('*');
        term();
    }
}


void Parser::mul()
{
    if (is_end) throw std::exception();

    if (cur == '(')
    {
        /*<*/
        int save = global_term_counter;
        /*>*/

        closed_expression();

        /*<*/
        scope_flag = global_term_counter - save == 1 ? 0 : 1;
        global_term_counter = save;
        /*>*/
    }
    else if (std::isalpha(cur))
    {
        /*<*/
        write->push_back(cur);
        scope_flag = 0;
        /*>*/

        match(cur);
    }
    else
    {
        throw std::exception();
    }
}


void Parser::closed_expression()
{
    match('(');
    expression();
    match(')');
}


void Parser::reset()
{
    str_ = nullptr;
    pos = 0;
    is_end = true;
    cur = '\0';
    local_mul_counter = nullptr;
    global_term_counter = 0;
    write = &result;
    scope_flag = 0;
    result.clear();
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


// a+b+c*b*s*(a+(b+d*(a+w)*(q+w)))
// (((a+b)*c)+b+c+(b+c))*d+a+b+a*b*(a*b+c)
