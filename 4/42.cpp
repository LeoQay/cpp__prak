#include <string>
#include <cstring>

/*
void fill_by_different_strings(std::string *buf, size_t size, size_t len);


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

 */


class PtrCharArr
{
public:
    explicit PtrCharArr(char * p);
    PtrCharArr(const PtrCharArr & obj) = delete;
    PtrCharArr & operator= (const PtrCharArr & obj) = delete;
    ~PtrCharArr();
    char * get() const;
    void set(char * p);
private:
    char * ptr = nullptr;
};

PtrCharArr::PtrCharArr(char * p) : ptr(p) {}

PtrCharArr::~PtrCharArr()
{
    delete [] ptr;
}

char * PtrCharArr::get() const
{
    return ptr;
}

void PtrCharArr::set(char * p)
{
    delete [] ptr;
    ptr = p;
}


class Action
{
public:
    virtual void run() = 0;
    virtual ~Action() = default;
};


class TokenizedAction
{
public:
    TokenizedAction(TokenPool & _pool, Action & _action);
    TokenizedAction(const TokenizedAction & obj) = delete;
    TokenizedAction & operator= (const TokenizedAction & obj) = delete;
    ~TokenizedAction();
    void run();
private:
    TokenPool & pool;
    Action & action;
    PtrCharArr token;
    size_t len = 0;
};

TokenizedAction::~TokenizedAction()
{
    pool.return_token(token.get());
}

TokenizedAction::TokenizedAction(TokenPool & _pool, Action & _action) :
pool(_pool), action(_action), token(nullptr)
{
    len = pool.token_length();
    token.set(new char[len + 1]);
    pool.get_token(token.get());
}

void TokenizedAction::run()
{
    action.run();
}


