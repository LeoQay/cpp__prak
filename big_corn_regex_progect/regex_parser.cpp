#include <stdexcept>
#include <vector>
#include <iostream>

#include "regex_parser.h"



RegexParser::RegexParser() : iterator(),
poliz(), bar_flag(false), concat_flag(false), is_empty(false) {}


void RegexParser::parse(std::istream & _stream)
{
    /*<*/
    poliz.clear();
     /*>*/

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
    /*<*/
    bool prev_bar_flag = bar_flag;
    bar_flag = false;
    /*>*/

    Member();

    /*<*/
    bar_flag = prev_bar_flag;

    if (is_empty)
    {
        PolizItem empty = {PolizItem::Kind::EMPTY, '\0'};
        poliz.push_back(empty);
    }
    /*>*/

    T();

    /*<*/
    bar_flag = false;
    /*>*/
}


void RegexParser::T()
{
    /*<*/
    if (bar_flag)
    {
        PolizItem bar = {PolizItem::Kind::OR, '|'};
        poliz.push_back(bar);
    }
    bar_flag = true;
    /*>*/

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

        /*<*/
        bool prev_concat_flag = concat_flag;
        concat_flag = false;
        /*>*/

        Coefficient();

        /*<*/
        concat_flag = prev_concat_flag;

        if (concat_flag)
        {
            PolizItem concat = {PolizItem::Kind::CONCAT, '\0'};
            poliz.push_back(concat);
        }
        concat_flag = true;
        /*>*/

        Member();

        /*<*/
        is_empty = false;
        /*>*/
    }
    else
    {
        // M -> EMPTY

        /*<*/
        concat_flag = false;
        is_empty = true;
        /*>*/
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
    if (iterator->kind == Token::Kind::PLUS)
    {
        // P -> '+'P

        /*<*/
        PolizItem plus = {PolizItem::Kind::ITER_PLUS, '+'};
        poliz.push_back(plus);
        /*>*/

        match(Token::Kind::PLUS, "Never reached");
        P();
    }
    else if (iterator->kind == Token::Kind::STAR)
    {
        // P -> '*'P

        /*<*/
        PolizItem star = {PolizItem::Kind::ITER_STAR, '*'};
        poliz.push_back(star);
        /*>*/

        match(Token::Kind::STAR, "Never reached");
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
    else if (iterator->kind == Token::Kind::CHAR)
    {
        // E -> 'terminal'

        /*<*/
        PolizItem character = {PolizItem::Kind::CHAR, iterator->repr};
        poliz.push_back(character);
        /*>*/

        match(Token::Kind::CHAR, "Never reached");
    }
    else if (iterator->kind == Token::Kind::DIGIT)
    {
        // E -> '\d'

        /*<*/
        PolizItem digit = {PolizItem::Kind::DIGIT, iterator->repr};
        poliz.push_back(digit);
        /*>*/

        match(Token::Kind::DIGIT, "Never reached");
    }
    else
    {
        throw std::runtime_error("Empty element");
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

std::vector<PolizItem>::const_iterator RegexParser::begin() const
{
    return poliz.begin();
}

std::vector<PolizItem>::const_iterator RegexParser::end() const
{
    return poliz.end();
}

