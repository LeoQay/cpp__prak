#include <cstdlib>
#include <cstring>

class DynArray
{
public:
    DynArray();
    DynArray(const DynArray & obj);
    ~DynArray();
    DynArray & operator= (const DynArray & obj);
    size_t len() const;
    void add(int v);
    void remove(size_t i);
    int & operator[] (size_t i);
    int operator[] (size_t i) const;
private:
    int *arr = nullptr;
    size_t size = 0;
    size_t real_size = 10;
};

DynArray::DynArray()
{
    arr = static_cast<int *>(malloc(real_size * sizeof *arr));
    memset(arr, '\0', size * sizeof *arr);
}

DynArray::DynArray(const DynArray & obj)
{
    *this = obj;
}

DynArray & DynArray::operator= (const DynArray & obj)
{
    if (this == &obj)
    {
        return *this;
    }

    if (arr != nullptr)
    {
        free(arr);
    }

    size = real_size = obj.size;
    arr = static_cast<int *>(malloc(real_size * sizeof *arr));
    memcpy(arr, obj.arr, size * sizeof *arr);

    return *this;
}

DynArray::~DynArray()
{
    free(arr);
}

size_t DynArray::len() const
{
    return size;
}

void DynArray::add(int v)
{
    if (size >= real_size)
    {
        real_size *= 2;
        arr = static_cast<int *>(realloc(arr, real_size * sizeof *arr));
    }

    arr[size++] = v;
}

void DynArray::remove(size_t i)
{
    memmove(arr + i, arr + i + 1, (size - i - 1) * sizeof *arr);
    size--;
}

int & DynArray::operator[] (size_t i)
{
    return arr[i];
}

int DynArray::operator[] (size_t i) const
{
    return arr[i];
}
