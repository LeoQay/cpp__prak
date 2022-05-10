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
    void A();
    void B();

    void match(char sym);
    void next();

    bool is_end();

    char cur;
    std::istream * stream;
};

// не важно, что лежит в cur изначально, так как перед
// первым обращением к нему всегда будет чтение, а если оно не успешно,
// то к cur не обратятся
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
    if (is_end() || cur == 'a' || cur == 'd')
    {
        // S -> B
        B();
    }
    else if (cur == 'b' || cur == 'c')
    {
        // S -> AS
        A();
        S();
    }
    else
    {
        throw std::exception();
    }
}


void Parser::A()
{
    if (is_end() || (cur != 'b' && cur != 'c'))
    {
        throw std::exception();
    }
    else
    {
        // A -> b | c
        match(cur);
    }
}


void Parser::B()
{
    if (is_end() || (cur != 'a' && cur != 'd'))
    {
        // B -> EMPTY
    }
    else if (cur == 'a')
    {
        // B -> a
        match('a');
    }
    else
    {
        // B -> dBf
        match('d');
        B();
        match('f');
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
