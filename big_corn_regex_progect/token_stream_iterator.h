#ifndef INC_93_TOKEN_STREAM_ITERATOR_H
#define INC_93_TOKEN_STREAM_ITERATOR_H

#include <stdexcept>


struct Token { enum Kind { OPEN, CLOSE, STAR, PLUS, BAR, DIGIT, CHAR, END } kind; char repr; };


class TokenStreamIterator
{
public:
    TokenStreamIterator(std::istream & _stream);
    TokenStreamIterator() noexcept;

    TokenStreamIterator(const TokenStreamIterator & iter) noexcept = default;
    TokenStreamIterator & operator= (const TokenStreamIterator &) = default;

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

    std::istream * stream;
    Token current;
    char sym;
};


#endif //INC_93_TOKEN_STREAM_ITERATOR_H
