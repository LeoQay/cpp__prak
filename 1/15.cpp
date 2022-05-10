#include <iostream>


class Printer
{
public:
    explicit Printer(std::ostream & s);
    Printer(const Printer & obj);
    ~Printer();
    Printer & operator<< (const std::string & str);
    unsigned long num_prints() const;
    static unsigned long num_objects();
private:
    static unsigned long obj_amount;
    unsigned long str_amount = 0;
    std::ostream & stream;
};

unsigned long Printer::obj_amount = 0;

Printer::Printer(const Printer & obj) : str_amount(0), stream(obj.stream)
{
    obj_amount++;
}

Printer::~Printer()
{
    obj_amount--;
}

Printer::Printer(std::ostream & s) : stream(s)
{
    obj_amount++;
}

Printer & Printer::operator<< (const std::string & str)
{
    str_amount++;
    stream << str << std::endl;
    return *this;
}

unsigned long Printer::num_prints() const
{
    return str_amount;
}

unsigned long Printer::num_objects()
{
    return obj_amount;
}

int main()
{

}
