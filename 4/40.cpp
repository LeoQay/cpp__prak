/*
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <stdexcept>

class E
{
public:
    E(): msg(0) {}
    E(char *s): msg(s) {}
    E(const E &oth): msg (static_cast<char *>(malloc(strlen(oth.msg) + 1))) {
        strcpy(msg, oth.msg);
    }
    E &operator=(const E &) = delete;
    ~E() { free(msg); }
    const char *message() const { return msg; }
private:
    char *msg;
};

*/

#include <string>

class Error
{
public:
    Error() : msg(*(new const std::string(""))) {}

    Error(const std::string & s) : msg(*(new const std::string(s))) {}

    Error(const Error & oth) : msg(*(new const std::string(oth.msg))) {}

    Error & operator=(const Error &oth) = delete;

    ~Error()
    {
        delete &msg;
    }

    const std::string & message() const
    {
        return msg;
    }
private:
    const std::string &msg;
};


/*
void
print_sum(std::istream &in, std::ostream &out)
{
    std::string s1;
    if (!std::getline(in, s1)) {
        return;
    }
    std::string s2;
    if (!std::getline(in, s2)) {
        throw Error("Incomplete pair");
    }
    size_t pos;
    int i1 = std::stoi(s1, &pos);
    if (pos != s1.length()) {
        std::string message = "The string has garbage: <<" + s1 + ">>";
        throw Error(message);
    }
    int i2 = std::stoi(s2, &pos);
    if (pos != s2.length()) {
        std::string message = "The string has garbage: <<" + s2 + ">>";
        throw Error(message);
    }
    out << (i1 + i2) << std::endl;
}

void
print_sums(std::istream &in, std::ostream &out)
{
    try {
        while (in) {
            print_sum(in, out);
        }
    } catch (std::logic_error &error) {
        std::string message(error.what());
        throw Error(message);
    }
}

int main()
{
    try {
        print_sums(std::cin, std::cout);
    } catch (E e) {
        std::cerr << e.message() << std::endl;
    } catch (Error error) {
        std::cerr << error.message() << std::endl;
    }
}

*/