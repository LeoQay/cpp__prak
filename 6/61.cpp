#include <string>
#include <stdexcept>
#include <cstring>
#include <unordered_map>
#include <vector>

// void fill_by_different_strings(std::string *, size_t, size_t);

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
    std::unordered_map<std::string, size_t> is_busy;
    std::vector<size_t> not_busy;
};

TokenPool::~TokenPool()
{
    delete [] arr;
}

TokenPool::TokenPool(size_t s) : size(s), is_busy(), not_busy()
{
    try {
        arr = new std::string[size];
        fill_by_different_strings(arr, size, len);
        for (size_t i = 0; i < size; i++)
        {
            not_busy.push_back(i);
            is_busy[arr[i]] = size;
        }
    }
    catch (std::exception &)
    {
        delete [] arr;
        throw;
    }
}

size_t TokenPool::token_length() const
{
    return len;
}

void TokenPool::get_token(char * addr)
{
    if (not_busy.empty())
    {
        throw std::exception();
    }

    size_t pos = not_busy.back();

    std::string & str = arr[pos];

    is_busy[str] = pos;

    try {
        not_busy.pop_back();
    }
    catch (std::exception &)
    {
        is_busy[str] = size;
        throw;
    }

    strcpy(addr, str.c_str());
}

void TokenPool::return_token(const char * addr)
{
    std::string str = addr;

    if (is_busy.find(str) == is_busy.end())
    {
        throw std::exception();
    }

    size_t pos = is_busy[str];

    if (pos == size)
    {
        throw std::exception();
    }

    is_busy[str] = size;

    try {
        not_busy.push_back(pos);
    }
    catch (std::exception &)
    {
        is_busy[str] = pos;
        throw;
    }
}

