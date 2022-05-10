#include <iostream>
#include <fstream>

class Person
{
public:
    virtual std::string name() const = 0;
    friend std::ostream & operator<< (std::ostream & s, const Person & obj);
    virtual ~Person();
protected:
    virtual std::string output() const = 0;
};

std::ostream & operator<< (std::ostream & stream, const Person & obj)
{
    stream << obj.output();
    return stream;
}

Person::~Person() {}

class NamedPerson : public Person
{
public:
    NamedPerson(const std::string & first_name, const std::string & last_name);
    std::string get_first_name() const;
    std::string get_last_name() const;
    std::string name() const;
protected:
    virtual std::string output() const;
private:
    std::string f_name;
    std::string l_name;
};

std::string NamedPerson::output() const
{
    return name();
}

NamedPerson::NamedPerson(const std::string & first_name,
        const std::string & last_name)
: f_name(first_name), l_name(last_name) {}

std::string NamedPerson::get_first_name() const
{
    return f_name;
}

std::string NamedPerson::get_last_name() const
{
    return l_name;
}

std::string NamedPerson::name() const
{
    std::string result = f_name + " " + l_name;
    return result;
}

class MysteryPerson : public NamedPerson
{
public:
    MysteryPerson(const std::string & first_name, const std::string & last_name,
            std::istream & stream);
    std::string spell() const;
protected:
    std::string output() const;
private:
    std::string sp;
};

std::string MysteryPerson::output() const
{
    return spell();
}

MysteryPerson::MysteryPerson(const std::string & first_name,
        const std::string & last_name, std::istream & stream)
: NamedPerson(first_name, last_name), sp("")
{
    stream >> sp;
}

std::string MysteryPerson::spell() const
{
    return sp;
}

class BossPerson : public Person
{
public:
    std::string name() const;
protected:
    std::string output() const;
};

std::string BossPerson::output() const
{
    return name();
}

std::string BossPerson::name() const
{
    std::string result = "I am the BOSS!";
    return result;
}

int main(int argc, const char **argv)
{
    Person * arr[3];
    arr[0] = new NamedPerson("James", "Brown");
    arr[1] = new BossPerson();
    arr[2] = new MysteryPerson("Mystery", "M", std::cin);

    for (int i = 0; i < 3; i++)
    {
        std::cout << arr[i]->name() << std::endl;
    }

    if (argc == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            delete arr[i];
        }
        return 0;
    }

    std::ofstream fout;

    fout.open(argv[1]);

    if(!fout.is_open())
    {
        for (int i = 0; i < 3; i++)
        {
            delete arr[i];
        }
        return 0;
    }

    fout << *arr[0] << " " << *arr[1] << " " << *arr[2] << std::endl;

    fout.close();

    for (int i = 0; i < 3; i++)
    {
        delete arr[i];
    }

    return 0;
}
