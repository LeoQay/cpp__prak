#include <string>
#include <cstring>

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


template<class T>
class TokenizedAction
{
public:
    TokenizedAction(TokenPool & _pool, T & _action);
    TokenizedAction(const TokenizedAction & obj) = delete;
    TokenizedAction & operator= (const TokenizedAction & obj) = delete;
    ~TokenizedAction();
    void run();
private:
    TokenPool & pool;
    T & action;
    PtrCharArr token;
    size_t len = 0;
};

template<class T>
TokenizedAction<T>::~TokenizedAction()
{
    pool.return_token(token.get());
}

template<class T>
TokenizedAction<T>::TokenizedAction(TokenPool & _pool, T & _action) :
pool(_pool), action(_action), token(nullptr)
{
    len = pool.token_length();
    token.set(new char[len + 1]);
    pool.get_token(token.get());
}

template<class T>
void TokenizedAction<T>::run()
{
    action.run();
}

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
