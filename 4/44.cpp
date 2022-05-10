#include <string>
#include <typeinfo>
#include <iostream>

class Data
{
public:
    virtual Data * clone() const = 0;
    virtual ~Data() = default;
};


class Number : public Data
{
public:
    Number(int n);
    int value() const;
    Number * clone() const override;
private:
    int num;
};

Number::Number(int n) : num(n) {}

int Number::value() const
{
    return num;
}

Number * Number::clone() const
{
    return new Number(num);
}


class Text : public Data
{
public:
    Text(std::string t);
    std::string value() const;
    Text * clone() const;
private:
    std::string text;
};

Text::Text(std::string t) : text(t) {}

std::string Text::value() const
{
    return text;
}

Text * Text::clone() const
{
    return new Text(text);
}


class DynPtrDataArr
{
public:
    explicit DynPtrDataArr(Data ** p);
    DynPtrDataArr(const DynPtrDataArr & obj) = delete;
    DynPtrDataArr & operator= (const DynPtrDataArr & obj) = delete;
    ~DynPtrDataArr();
    void push(Data * data);
    Data * pop();
    size_t get_current_size() const;
private:
    Data ** ptr = nullptr;
    size_t cur_size = 0;
};

DynPtrDataArr::DynPtrDataArr(Data ** p) : ptr(p) {}

DynPtrDataArr::~DynPtrDataArr()
{
    for (size_t i = 0; i < cur_size; i++)
    {
        delete ptr[i];
    }
    delete [] ptr;
}

void DynPtrDataArr::push(Data * data)
{
    ptr[cur_size++] = data;
}

Data * DynPtrDataArr::pop()
{
    return ptr[--cur_size];
}

size_t DynPtrDataArr::get_current_size() const
{
    return cur_size;
}


class BoundedStack
{
public:
    BoundedStack(size_t s);
    void push(const Data & data);
    Data * pop();
private:
    size_t size;
    DynPtrDataArr arr;
};

BoundedStack::BoundedStack(size_t s) : size(s), arr(new Data*[s]) {}

void BoundedStack::push(const Data & data)
{
    if (arr.get_current_size() >= size)
    {
        throw std::exception();
    }
    arr.push(data.clone());
}

Data * BoundedStack::pop()
{
    if (arr.get_current_size() == 0)
    {
        throw std::exception();
    }
    return arr.pop();
}


class Instruction;
class StackInterpreter;


class StackInterpreter : public BoundedStack
{
public:
    StackInterpreter(size_t s);

    StackInterpreter(const StackInterpreter & obj) = delete;
    StackInterpreter & operator= (const StackInterpreter & obj) = delete;
};

StackInterpreter::StackInterpreter(size_t s) : BoundedStack(s) {}


Number * pop_and_get_number(StackInterpreter & obj)
{
    Data * data;
    Number * num;

    data = obj.pop();
    num = dynamic_cast<Number *>(data);

    if (num == nullptr)
    {
        obj.push(*data);
        delete data;
        throw std::bad_cast();
    }

    return num;
}


class Instruction
{
public:
    virtual void run(StackInterpreter & obj) = 0;
    virtual ~Instruction() = default;
};


class AddInstruction : public Instruction
{
public:
    void run(StackInterpreter & obj) override;
};

void AddInstruction::run(StackInterpreter & obj)
{
    Number * first, * second;

    first = pop_and_get_number(obj);
    try
    {
        second = pop_and_get_number(obj);
    }
    catch (std::exception &)
    {
        obj.push(*first);
        delete first;
        throw;
    }

    try
    {
        obj.push(Number(first->value() + second->value()));
    }
    catch (std::exception &)
    {
        obj.push(*second);
        obj.push(*first);
        delete first;
        delete second;
        throw;
    }

    delete first;
    delete second;
}

/*
int main()
{
    Number * num = nullptr;
    try
    {
        StackInterpreter obj(10);
        AddInstruction ins;

        obj.push(Number(1));
        obj.push(Number(2));

        //obj.push(Text(std::string("Haha")));

        ins.run(obj);

        num = pop_and_get_number(obj);

        std::cout << num->value() << std::endl;

        delete num;
    }
    catch (std::exception &)
    {
        delete num;
    }



    return 0;
}
*/