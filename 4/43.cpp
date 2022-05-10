#include <string>


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
