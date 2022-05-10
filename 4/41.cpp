#include <string>
#include <exception>
#include <cstring>

/*
class PtrStringArr
{
public:
    explicit PtrStringArr(std::string * p);
    PtrStringArr(const PtrStringArr & obj) = delete;
    PtrStringArr & operator= (const PtrStringArr & obj) = delete;
    ~PtrStringArr();
    std::string * get() const;
private:
    std::string * ptr = nullptr;
};

PtrStringArr::PtrStringArr(std::string * p) : ptr(p) {}

PtrStringArr::~PtrStringArr()
{
    delete [] ptr;
}

std::string * PtrStringArr::get() const
{
    return ptr;
}

class PtrBoolArr
{
public:
    explicit PtrBoolArr(bool * p);
    PtrBoolArr(const PtrBoolArr & obj) = delete;
    PtrBoolArr & operator= (const PtrBoolArr & obj) = delete;
    ~PtrBoolArr();
    bool * get() const;
private:
    bool * ptr = nullptr;
};

PtrBoolArr::PtrBoolArr(bool * p) : ptr(p) {}

PtrBoolArr::~PtrBoolArr()
{
    delete [] ptr;
}

bool * PtrBoolArr::get() const
{
    return ptr;
}
*/


class TokenPool
{
public:
    explicit TokenPool(size_t s);
    TokenPool(const TokenPool & obj) = delete;
    TokenPool & operator= (const TokenPool & obj) = delete;
    ~TokenPool();
    size_t token_length() const;
    void get_token(char * addr);
    void return_token(const char * addr);
private:
    size_t len = 10;
    size_t size;
    std::string * arr = nullptr;
    bool * is_free = nullptr;
};

TokenPool::~TokenPool()
{
    delete [] arr;
    delete [] is_free;
}

TokenPool::TokenPool(size_t s) : size(s)
{
    try {
        arr = new std::string[size];
        is_free = new bool[size];
        fill_by_different_strings(arr, size, len);
        for (size_t i = 0; i < size; i++)
        {
            is_free[i] = true;
        }
    }
    catch (std::bad_alloc &)
    {
        delete [] arr;
        delete [] is_free;
        throw;
    }
    catch (std::exception &)
    {
        delete [] arr;
        delete [] is_free;
        throw;
    }
}

size_t TokenPool::token_length() const
{
    return len;
}

void TokenPool::get_token(char * addr)
{
    size_t i;
    for (i = 0; i < size; i++)
    {
        if (is_free[i])
        {
            break;
        }
    }
    if (i == size)
    {
        throw std::exception();
    }
    is_free[i] = false;
    strcpy(addr, arr[i].c_str());
}

void TokenPool::return_token(const char * addr)
{
    for (size_t i = 0; i < size; i++)
    {
        if (strcmp(addr, arr[i].c_str()) == 0)
        {
            if (is_free[i])
            {
                throw std::exception();
            }
            is_free[i] = true;
            return;
        }
    }

    throw std::exception();
}

