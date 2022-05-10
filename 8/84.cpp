#include <iostream>
#include <stdexcept>


enum token_kind {
    OPEN_BRACKET = '(',
    CLOSE_BRACKET = ')',
    STAR = '*',
    PLUS = '+',
    STICK = '|',
    NUM,
    TERMINAL,
    END
};


struct Token
{
    Token(token_kind _kind = END, char _repr = EOF) noexcept;
    token_kind kind;
    char repr;
};

Token::Token(token_kind _kind, char _repr) noexcept
: kind(_kind), repr(_repr) {}


class TokenStreamIterator
{
public:
    TokenStreamIterator(std::istream & _stream);
    TokenStreamIterator() noexcept;

    TokenStreamIterator(const TokenStreamIterator & iter) noexcept = default;
    TokenStreamIterator & operator= (const TokenStreamIterator &) = delete;

    bool operator== (const TokenStreamIterator & iter) const noexcept;
    bool operator!= (const TokenStreamIterator & iter) const noexcept;

    const Token & operator* () const noexcept;
    const Token * operator-> () const noexcept;

    TokenStreamIterator & operator++ ();
    TokenStreamIterator operator++ (int);

private:
    void next_token();
    bool next_char_not_space();
    bool next_char_any();

    std::istream & stream;
    Token current;
    char sym;
};

TokenStreamIterator::TokenStreamIterator(std::istream & _stream)
: stream(_stream), current(TERMINAL), sym(EOF)
{
    next_token();
}

TokenStreamIterator::TokenStreamIterator() noexcept
: stream(std::cin), current(END), sym(EOF) {}

bool TokenStreamIterator::operator== (const TokenStreamIterator & iter) const noexcept
{
    return (current.kind == END && iter.current.kind == END) ||
           (current.kind != END && iter.current.kind != END &&
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
    if (current.kind == END)
    {
        return;
    }

    if (!next_char_not_space())
    {
        current = END;
        return;
    }

    switch (sym)
    {
        case OPEN_BRACKET:
            current = {OPEN_BRACKET, sym};
            return;
        case CLOSE_BRACKET:
            current = {CLOSE_BRACKET, sym};
            return;
        case STAR:
            current = {STAR, sym};
            return;
        case PLUS:
            current = {PLUS, sym};
            return;
        case STICK:
            current = {STICK, sym};
            return;
        case '\\':
            break;
        default:
            current = {TERMINAL, sym};
            return;
    }

    // yoy will be here only after backslash

    if (!next_char_any())
    {
        current = END;
        throw std::exception();
    }

    if (std::isspace(sym))
    {
        current = {TERMINAL, sym};
        return;
    }

    switch (sym) {
        case 'd':
            current = {NUM, sym};
            return;
        case OPEN_BRACKET:
        case CLOSE_BRACKET:
        case STAR:
        case PLUS:
        case STICK:
        case '\\':
            current = {TERMINAL, sym};
            return;
        default:
            throw std::exception();
    }
}

bool TokenStreamIterator::next_char_not_space()
{
    while (true)
    {
        bool is_not_end = static_cast<bool>(stream.get(sym));

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
    return static_cast<bool>(stream.get(sym));
}



/*
int main()
{

    TokenStreamIterator obj(std::cin);

    while (obj.current.kind != END)
    {
        ++obj;
    }

    return 0;
}
*/
