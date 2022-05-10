#include <iostream>
#include <string>
#include <map>
#include <sstream>


enum States {LVALUE, RVALUE};

struct Operand
{
    Operand() : state(RVALUE), value(0) {}
    States state;
    int value;
};


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

    void assignment();
    void assignment_continue_1();
    void assignment_continue_2();

    void calc();
    void calc_continue();

    void sop();
    void var();
    void op();

    void match(char sym);
    void next();
    void reset();


    int get_rvalue(const Operand & getter);
    void set_to_lvalue(const Operand & getter, const Operand & setter);


    bool is_end;
    char cur;
    size_t pos;
    const std::string * str_;


    Operand result;
    Operand * operand;
    std::map<int, int> dict;
};


Evaluator::Evaluator() :
is_end(true), cur('\0'), pos(0), str_(nullptr),
result(), operand(nullptr), dict() {}


bool Evaluator::parse(std::string & str)
{
    reset();
    is_end = false;
    str_ = &str;

    int res;

    try {
        next();
        expression();
        if (!is_end) throw std::exception();
        res = get_rvalue(result);
    }
    catch (std::exception &)
    {
        reset();
        return false;
    }


    std::stringstream stream;
    stream << res;
    str = stream.str();
    reset();
    return true;
}


void Evaluator::expression()
{
    assignment();
    expression_continue();
}


void Evaluator::expression_continue()
{
    if (is_end || cur != ',')
    {
        // comp_continue -> EMPTY
    }
    else
    {
        /*<*/
        get_rvalue(*operand);
        /*>*/

        match(',');
        expression();
    }
}


void Evaluator::assignment()
{
    if (is_end) throw std::exception();

    if (std::isalpha(cur))
    {
        var();
        assignment_continue_1();
    }
    else
    {
        sop();
        assignment_continue_2();
    }
}


void Evaluator::assignment_continue_1()
{
    if (is_end || cur != '=')
    {
        assignment_continue_2();
    }
    else
    {
        /*<*/
        Operand * prev = operand, temp;
        operand = &temp;
        /*>*/

        match('=');
        assignment();

        /*<*/
        set_to_lvalue(*prev, temp);
        operand = prev;
        /*>*/
    }
}


void Evaluator::assignment_continue_2()
{
    if (is_end || cur != '-')
    {
        // assignment_continue_2 -> EMPTY
    }
    else
    {
        /*<*/
        Operand * prev = operand, temp;
        operand = &temp;
        /*>*/

        match('-');
        calc();

        /*<*/
        int left = get_rvalue(*prev);
        int res = get_rvalue(*operand);
        operand = prev;
        operand->state = RVALUE;
        operand->value = left - res;
        /*>*/
    }
}


void Evaluator::calc()
{
    op();
    calc_continue();
}


void Evaluator::calc_continue()
{
    if (is_end || cur != '-')
    {
        // calc_continue -> EMPTY
    }
    else
    {
        /*<*/
        Operand * prev = operand, temp;
        operand = &temp;
        /*>*/

        match('-');
        calc();

        /*<*/
        int left = get_rvalue(*prev);
        int res = get_rvalue(*operand);
        operand = prev;
        operand->state = RVALUE;
        operand->value = left + res;
        /*>*/
    }
}


void Evaluator::sop()
{
    if (is_end)
    {
        throw std::exception();
    }

    if (std::isdigit(cur))
    {
        /*<*/
        operand->state = RVALUE;
        operand->value = cur - '0';
        /*>*/

        match(cur);
    }
    else if (cur == '(')
    {
        match('(');
        expression();
        match(')');

        /*<*/
        operand->value = get_rvalue(*operand);
        operand->state = RVALUE;
        /*>*/
    }
    else
    {
        throw std::exception();
    }
}


void Evaluator::var()
{
    if (is_end || !std::isalpha(cur))
    {
        throw std::exception();
    }

    /*<*/
    operand->state = LVALUE;
    operand->value = static_cast<unsigned char>(cur);
    /*>*/

    match(cur);
}


void Evaluator::op()
{
    if (is_end) throw std::exception();

    if (std::isalpha(cur))
    {
        var();
    }
    else
    {
        sop();
    }
}



void Evaluator::reset()
{
    str_ = nullptr;
    pos = 0;
    is_end = true;
    cur = '\0';

    result.state = RVALUE;
    result.value = 0;
    operand = &result;
    dict.clear();
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


int Evaluator::get_rvalue(const Operand & getter)
{
    if (getter.state == RVALUE)
    {
        return getter.value;
    }
    else
    {
        auto it = dict.find(getter.value);

        if (it == dict.end())
        {
            throw std::exception();
        }

        return it->second;
    }
}


void Evaluator::set_to_lvalue(const Operand & getter, const Operand & setter)
{
    if (getter.state != LVALUE) throw std::exception();
    dict[getter.value] = get_rvalue(setter);
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
