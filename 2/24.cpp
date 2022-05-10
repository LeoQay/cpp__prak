#include <iostream>

class ItemizedLogger
{
public:
    ItemizedLogger(std::ostream & s);
    void log(const std::string & msg);
    bool fail() const;
protected:
    std::ostream & stream;
private:
    size_t num = 0;
};

ItemizedLogger::ItemizedLogger(std::ostream & s) : stream(s) {}

void ItemizedLogger::log(const std::string & msg)
{
    num++;
    stream << num << ". " << msg << std::endl;
}

bool ItemizedLogger::fail() const
{
    return stream.fail();
}


class PrefixedItemizedLogger : public ItemizedLogger
{
public:
    PrefixedItemizedLogger(std::ostream & s, const std::string & pref);
    void log(const std::string & msg);
private:
    const std::string prefix;
};

PrefixedItemizedLogger::PrefixedItemizedLogger(std::ostream & s, const std::string & pref) : ItemizedLogger(s), prefix(pref) {}

void PrefixedItemizedLogger::log(const std::string & msg)
{
    stream << prefix << ": ";
    ItemizedLogger::log(msg);
}

int main()
{
    PrefixedItemizedLogger obj(std::cout, "Prefix");

    obj.log("Hhaha");

    ItemizedLogger & obj2 = obj;

    obj2.log("Hhaha");
}