#include <iostream>

#include "regex_parser.h"
#include "token_stream_iterator.h"



RegexParser::RegexParser() : iterator() {}


void RegexParser::parse(std::istream & _stream)
{
    iterator = TokenStreamIterator(_stream);

    RegularExpression();

    if (!is_end())
    {
        throw std::runtime_error("Regular expression done, "
                                 "but stream is not empty");
    }
}


void RegexParser::RegularExpression()
{
    Member();
    T();
}


void RegexParser::T()
{
    if (iterator->kind == Token::Kind::BAR)
    {
        // T -> '|'R
        match(Token::Kind::BAR, "Never reached");
        RegularExpression();
    }
    else
    {
        // T -> EMPTY
    }
}


void RegexParser::Member()
{
    if (iterator->kind == Token::Kind::OPEN ||
        iterator->kind == Token::Kind::CHAR ||
        iterator->kind == Token::Kind::DIGIT)
    {
        // M -> CM
        Coefficient();
        Member();
    }
    else
    {
        // M -> EMPTY
    }
}

void RegexParser::Coefficient()
{
    // C -> EP
    Element();
    P();
}

void RegexParser::P()
{
    if (iterator->kind == Token::Kind::PLUS ||
        iterator->kind == Token::Kind::STAR)
    {
        // P -> '+'P | '*'P
        match(iterator->kind, "Never reached");
        P();
    }
    else
    {
        // P -> EMPTY
    }
}


void RegexParser::Element()
{
    if (iterator->kind == Token::Kind::OPEN)
    {
        // E -> '('R')'
        match(Token::Kind::OPEN, "Never reached");
        RegularExpression();
        match(Token::Kind::CLOSE, "Missing close bracket");
    }
    else if (iterator->kind == Token::Kind::CHAR ||
             iterator->kind == Token::Kind::DIGIT)
    {
        // E -> '\d' | 'terminal'
        match(iterator->kind, "Never reached");
    }
}



void RegexParser::match(Token::Kind kind, const char * msg)
{
    if (iterator->kind != kind)
    {
        throw std::runtime_error(msg);
    }
    next();
}

void RegexParser::next()
{
    ++iterator;
}

bool RegexParser::is_end()
{
    return iterator->kind == Token::Kind::END;
}
