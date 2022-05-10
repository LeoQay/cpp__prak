/*
 * S → bS  | aAB
 * A → bcA | ccA | ε
 * B → cbB | ε
 *
 * Для данной грамматики не применим рекурсивный спуск:
 * follow(A)  = { c }
 * first(ccA) = { c }
 * first(ccA) AND follow(A) = { C } != EMPTY
 *
 * Используем следующую грамматику:
 * S → bS  | aT
 * T → bcT | cP | ε
 * P → cT  | bB
 * B → cbB | ε
 *
 * Была произведена замена выражения AB, которое гарантированно
 * раскрывается только один раз, на новый не терминал T.
 * И AB и T выводят цепочки следующего вида: (bc|cc)*(cb)*
 * S → bS  | aT
 * first(bS) = { b }
 * first(aT) = { a }
 *
 * Для предложенной грамматики применим метод рекурсивного спуска:
 *
 * T → bcT | cP | ε
 * first(bcT) = { b }
 * first(cP)  = { c }
 * first(ε) = EMPTY
 *
 * P → cT  | bB
 * first(cT) = { c }
 * first(bB) = { b }
 *
 * B → cbB | ε
 * first(cbB) = { c }
 * first(ε) = EMPTY
 *
 * follow(T) = EMPTY
 * follow(B) = EMPTY
 * не пустых пересечений не может быть
 *
 *
 */


#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>


class Parser
{
public:
    Parser();
    bool parse(std::istream & _stream);
private:
    void S();
    void T();
    void P();
    void B();

    void match(char sym);
    void next();

    bool is_end();

    char cur;
    std::istream * stream;
};


Parser::Parser() : cur('\0'), stream(nullptr) {}

void Parser::match(char sym)
{
    if (is_end() || cur != sym)
    {
        throw std::exception();
    }
    next();
}

void Parser::next()
{
    stream->get(cur);
}

bool Parser::is_end()
{
    return stream->fail() || stream->eof();
}


bool Parser::parse(std::istream & _stream)
{
    stream = &_stream;

    try {
        next();
        S();

        if (!is_end())
        {
            throw std::exception();
        }
    }
    catch (std::exception &)
    {
        stream = nullptr;
        return false;
    }

    stream = nullptr;
    return true;
}


void Parser::S()
{
    if (is_end())
    {
        throw std::exception();
    }

    if (cur == 'a')
    {
        // S -> aT
        match('a');
        T();
    }
    else if (cur == 'b')
    {
        // S -> bT
        match('b');
        S();
    }
    else
    {
        throw std::exception();
    }
}


void Parser::T()
{
    if (is_end())
    {
        // T -> EMPTY
    }
    else if (cur == 'b')
    {
        // T -> bcT
        match('b');
        match('c');
        T();
    }
    else if (cur == 'c')
    {
        // T -> cP
        match('c');
        P();
    }
    else
    {
        throw std::exception();
    }
}

void Parser::P()
{
    if (is_end())
    {
        throw std::exception();
    }

    if (cur == 'b')
    {
        // P -> cT
        match('b');
        B();
    }
    else if (cur == 'c')
    {
        // P -> bB
        match('c');
        T();
    }
    else
    {
        throw std::exception();
    }
}

void Parser::B()
{
    if (is_end())
    {
        // B -> EMPTY
    }
    else if (cur == 'c')
    {
        // B -> cbB
        match('c');
        match('b');
        B();
    }
    else
    {
        throw std::exception();
    }
}


int main()
{
    Parser parser;

    std::string str;

    while (getline(std::cin, str))
    {
        std::stringstream stream;
        stream << str;

        if (parser.parse(stream))
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