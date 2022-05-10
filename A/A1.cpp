#include <iostream>
#include <string>


class Evaluator
{
public:
    Evaluator();
    Evaluator(const Evaluator & obj) = delete;
    Evaluator & operator= (const Evaluator & obj) = delete;
    bool parse(std::string & str);
private:

    void expression();
    void expression_continue();
    void term();
    void term_continue();
    void mul();

    void match(char sym);
    void next();
    void reset();

    bool is_end;
    char cur;
    size_t pos;
    const std::string * str_;

    int mul_counter;
    int term_counter;
    int term_counter_forward;
    int miss_scope_flag;
    std::string * write;
    std::string result;
};


Evaluator::Evaluator() :
is_end(true), cur('\0'), pos(0), str_(nullptr),
mul_counter(0), term_counter(0), term_counter_forward(0),
miss_scope_flag(0), write(nullptr), result() {}


bool Evaluator::parse(std::string & str)
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


void Evaluator::expression()
{
    /*<*/
    mul_counter = 0;
    /*>*/

    term();

    /*<*/
    mul_counter = 0;
    term_counter++;
    /*>*/

    expression_continue();
}


void Evaluator::expression_continue()
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


void Evaluator::term()
{
    /*<*/
    std::string mul_buffer;
    std::string * prev_ptr = write;
    write = &mul_buffer;

    int save_term_counter = term_counter;
    int save_mul_counter = mul_counter;
    mul_counter = 0;
    term_counter = 0;
    /*>*/

    mul();

    /*<*/
    mul_counter = save_mul_counter;
    mul_counter++;
    term_counter_forward = term_counter;
    int save_mul_term_counter = term_counter;
    term_counter = save_term_counter;
    /*>*/

    term_continue();

    /*<*/
    if (miss_scope_flag)
    {
        term_counter += save_mul_term_counter - 1;
    }

    prev_ptr->append(*write);
    write = prev_ptr;
    /*>*/
}


void Evaluator::term_continue()
{
    if (is_end || cur != '*')
    {
        // subT -> EMPTY

        /*<*/
        miss_scope_flag = 0;

        if (mul_counter > 1 && term_counter_forward > 1)
        {
            write->insert(0, 1, '(');
            write->push_back(')');
        }
        else if (mul_counter == 1 && term_counter_forward > 0)
        {
            miss_scope_flag = 1;
        }
        /*>*/
    }
    else
    {
        /*<*/
        miss_scope_flag = 0;

        if (term_counter_forward > 1)
        {
            write->insert(0, 1, '(');
            write->push_back(')');
        }

        write->push_back('*');
        /*>*/

        match('*');
        term();
    }
}


void Evaluator::mul()
{
    if (is_end) throw std::exception();

    if (cur == '(')
    {
        match('(');
        expression();
        match(')');
    }
    else if (std::isalpha(cur))
    {
        /*<*/
        write->push_back(cur);
        /*>*/

        match(cur);
    }
    else
    {
        throw std::exception();
    }
}


void Evaluator::reset()
{
    str_ = nullptr;
    pos = 0;
    is_end = true;
    cur = '\0';
    mul_counter = 0;
    term_counter = 0;
    term_counter_forward = 0;
    miss_scope_flag = 0;
    write = &result;
    result.clear();
}


void Evaluator::match(char sym)
{
    if (is_end || cur != sym)
    {
        throw std::exception();
    }
    next();
}


void Evaluator::next()
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


/*
int main()
{
    Evaluator parser;

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
*/