#include <iostream>
#include <string>

/*
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
*/


template<typename T>
class SmartPointer
{
public:
    SmartPointer() = default;
    SmartPointer(T * p);
    SmartPointer(SmartPointer & obj);
    SmartPointer & operator= (SmartPointer & obj);
    SmartPointer & operator= (T *) = delete;
    ~SmartPointer();
    T & operator* ();
    T * operator-> ();
    operator T * ();

    template<typename U>
    SmartPointer<T> & operator= (SmartPointer<U> & obj);

    const T * pointer() const;
private:
    T * ptr = nullptr;
};

template<typename T>
const T * SmartPointer<T>::pointer() const
{
    return ptr;
}


template<typename T>
SmartPointer<T>::SmartPointer(T * p)
{
    ptr = p;
}

template<typename T>
SmartPointer<T>::SmartPointer(SmartPointer & obj)
{
    *this = obj;
}

template<typename T>
SmartPointer<T> & SmartPointer<T>::operator= (SmartPointer & obj)
{
    if (this == &obj)
    {
        return *this;
    }

    ptr = obj.ptr;
    obj.ptr = nullptr;

    return *this;
}

template<typename T>
T * SmartPointer<T>::operator->()
{
    return ptr;
}

template<typename T>
T & SmartPointer<T>::operator*()
{
    return *ptr;
}

template<typename T>
SmartPointer<T>::~SmartPointer()
{
    delete ptr;
}

template<typename T>
SmartPointer<T>::operator T *()
{
    T * ret = ptr;
    ptr = nullptr;
    return ret;
}

template <typename T>
template <typename U>
SmartPointer<T> & SmartPointer<T>::operator= (SmartPointer<U> & obj)
{
    if (ptr == obj.pointer())
    {
        return *this;
    }

    delete ptr;

    U * p = static_cast<U *>(obj);
    ptr = p;

    return *this;
}



void make(SmartPointer<Number> & obj, int value)
{
    SmartPointer<Number> temp(new Number(value));
    obj = temp;
}

void make(SmartPointer<Text> & obj, const std::string & value)
{
    SmartPointer<Text> temp(new Text(value));
    obj = temp;
}


/*
int main()
{
    Array<SmartPointer<Data>> array(10);

    SmartPointer<Number> n;
    make(n, 100); // makes Number(100) in n
    std::cout << "n = " << n->value() << std::endl;
    array[0] = n; // moves Number from n into array[0]

    SmartPointer<Number> m;
    make(m, 200); // makes Number(200) in m
    std::cout << "m = " << m->value() << std::endl;
    array[1] = m; // moves Number from m into array[1]

    SmartPointer<Number> k;
    make(k, 300); // makes Number(300) in k
    std::cout << "k = " << k->value() << std::endl;
    array[1] = k; // removes old Number in array[1]
    // and moves Number from k into array[1]

    SmartPointer<Text> t;
    make(t, "1111111111111111111111111111111"); // makes Text("1...") in t
    std::cout << "t = " << t->value() << std::endl;
    array[0] = t; // removes old Number in array[0]
    // and moves Text from t into array[0]

    Text &s = static_cast<Text &>(*array[0]); // reads array[0]
    std::cout << "s = " << s.value() << std::endl;    // prints its value
}

*/
