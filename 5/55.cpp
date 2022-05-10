#include <iostream>

template<typename T>
class Array
{
public:
    Array(size_t s);
    Array(Array & obj);
    Array & operator= (Array & obj);
    ~Array();
    size_t length() const;
    T & operator[] (size_t i);
private:
    size_t size = 0;
    T * array = nullptr;
};


template<typename T>
Array<T>::Array(size_t s) : size(s), array(new T[s]) {}

template<typename T>
Array<T>::Array(Array & obj)
{
    *this = obj;
}

template<typename T>
Array<T> & Array<T>::operator= (Array & obj)
{
    if (this == &obj)
    {
        return *this;
    }

    delete [] array;

    array = obj.array;
    size = obj.size;

    obj.array = nullptr;
    obj.size = 0;

    return *this;
}

template<typename T>
size_t Array<T>::length() const
{
    return size;
}

template<typename T>
T & Array<T>::operator[] (size_t i)
{
    if (size == 0 || i >= size)
    {
        throw std::exception();
    }

    return array[i];
}

template<typename T>
Array<T>::~Array()
{
    delete [] array;
}
