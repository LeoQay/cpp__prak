#include <iostream>
#include <stdexcept>

#include "token_stream_iterator.h"



TokenStreamIterator::TokenStreamIterator(std::istream & _stream)
: stream(&_stream), current({Token::Kind::CHAR, '\0'}), sym('\0')
{
    next_token();
}

TokenStreamIterator::TokenStreamIterator() noexcept
: stream(nullptr), current({Token::Kind::END, '\0'}), sym('\0') {}

bool TokenStreamIterator::operator== (const TokenStreamIterator & iter) const noexcept
{
    return (current.kind == Token::Kind::END && iter.current.kind == Token::Kind::END) ||
           (current.kind != Token::Kind::END && iter.current.kind != Token::Kind::END &&
            &stream == &iter.stream);
}

bool TokenStreamIterator::operator!= (const TokenStreamIterator & iter) const noexcept
{
    return !operator== (iter);
}

TokenStreamIterator & TokenStreamIterator::operator++()
{
    next_token();
    return *this;
}

TokenStreamIterator TokenStreamIterator::operator++(int)
{
    TokenStreamIterator save = *this;
    next_token();
    return save;
}

const Token & TokenStreamIterator::operator*() const noexcept
{
    return current;
}

const Token * TokenStreamIterator::operator->() const noexcept
{
    return &current;
}


void TokenStreamIterator::next_token()
{
    if (current.kind == Token::Kind::END)
    {
        return;
    }

    if (!next_char_not_space())
    {
        current = {Token::Kind::END, '\0'};
        return;
    }

    switch (sym)
    {
        case Token::Kind::OPEN:
            current = {Token::Kind::OPEN, sym};
            return;
        case Token::Kind::CLOSE:
            current = {Token::Kind::CLOSE, sym};
            return;
        case Token::Kind::STAR:
            current = {Token::Kind::STAR, sym};
            return;
        case Token::Kind::PLUS:
            current = {Token::Kind::PLUS, sym};
            return;
        case Token::Kind::BAR:
            current = {Token::Kind::BAR, sym};
            return;
        case '\\':
            break;
        default:
            current = {Token::Kind::CHAR, sym};
            return;
    }

    // yoy will be here only after backslash

    if (!next_char_any())
    {
        current = {Token::Kind::END, '\0'};
        throw std::exception();
    }

    if (std::isspace(sym))
    {
        current = {Token::Kind::CHAR, sym};
        return;
    }

    switch (sym) {
        case 'd':
            current = {Token::Kind::DIGIT, sym};
            return;
        case Token::Kind::OPEN:
        case Token::Kind::CLOSE:
        case Token::Kind::STAR:
        case Token::Kind::PLUS:
        case Token::Kind::BAR:
        case '\\':
            current = {Token::Kind::CHAR, sym};
            return;
        default:
            throw std::exception();
    }
}

bool TokenStreamIterator::next_char_not_space()
{
    while (true)
    {
        bool is_not_end = static_cast<bool>(stream->get(sym));

        if (!is_not_end)
        {
            return false;
        }

        if (!std::isspace(sym))
        {
            return true;
        }
    }
}

bool TokenStreamIterator::next_char_any()
{
    return static_cast<bool>(stream->get(sym));
}
