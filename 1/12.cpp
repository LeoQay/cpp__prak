#include <cstring>
#include <iostream>

class String1024
{
public:
    String1024();
    const char * get_pointer() const;
    friend String1024 operator+ (const String1024 & obj1,
                                   const String1024 & obj2);
    friend String1024 operator+ (const String1024 & obj, char sym);
    friend String1024 operator+ (char sym, const String1024 & obj);
    friend String1024 operator+ (const String1024 & obj, const char * str);
    friend String1024 operator+ (const char * str, const String1024 & obj);
    void print() const;
private:
    char buffer[1024];
    unsigned long len;
};

String1024::String1024() : len(0)
{
    buffer[0] = '\0';
}

const char * String1024::get_pointer() const
{
    return static_cast<const char *>(buffer);
}

String1024 operator+ (const String1024 & obj1, const String1024 & obj2)
{
    String1024 obj = obj1;
    memcpy(obj.buffer + obj.len, obj2.buffer, obj2.len + 1);
    obj.len += obj2.len;

    return obj;
}

String1024 operator+ (const String1024 & obj, char sym)
{
    String1024 result = obj;
    result.buffer[result.len] = sym;
    result.len++;
    result.buffer[result.len] = '\0';
    return result;

}

String1024 operator+ (char sym, const String1024 & obj)
{
    String1024 result;
    result = result + sym;
    result = result + obj;
    return result;
}

String1024 operator+ (const String1024 & obj, const char * str)
{
    String1024 result = obj;
    unsigned long len = strlen(str);
    memcpy(result.buffer + result.len, str, len + 1);
    result.len = obj.len + len;
    return result;
}

String1024 operator+ (const char * str, const String1024 & obj)
{
    String1024 result;
    result.len = strlen(str);
    memcpy(result.buffer, str, result.len + 1);
    result = result + obj;
    return result;
}

void String1024::print() const
{
    std::cout << buffer << std::endl;
}


