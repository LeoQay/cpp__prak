#ifndef MSU_CPP_PRAK_2022_REGEXPARSER_H
#define MSU_CPP_PRAK_2022_REGEXPARSER_H

#include "token_stream_iterator.h"

#include <iostream>


class RegexParser
{
public:
    RegexParser();
    void parse(std::istream & _stream);
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
};


#endif //MSU_CPP_PRAK_2022_REGEXPARSER_H
