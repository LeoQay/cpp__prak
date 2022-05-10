
#ifndef CPP__PRAK_REGEX_PARSER_H
#define CPP__PRAK_REGEX_PARSER_H


#include <vector>
#include <iostream>

#include "token_stream_iterator.h"


class RegexParser
{
public:
    RegexParser();
    void parse(std::istream & _stream);

    [[nodiscard]] std::vector<PolizItem>::const_iterator begin() const;
    [[nodiscard]] std::vector<PolizItem>::const_iterator end() const;
private:

    void RegularExpression();
    void T();
    void Member();
    void Coefficient();
    void P();
    void Element();

    void match(Token::Kind kind, const char * msg);
    void next();
    bool is_end();

    TokenStreamIterator iterator;

    std::vector<PolizItem> poliz;
    bool bar_flag;
    bool concat_flag;
    bool is_empty;
};


#endif //CPP__PRAK_REGEX_PARSER_H
