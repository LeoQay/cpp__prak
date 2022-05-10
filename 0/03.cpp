#include <cstring>
#include "unistd.h"
#include "fcntl.h"
#include <cstdlib>
#include <exception>
#include <cctype>

class StringRangeHolder
{
public:
    StringRangeHolder();
    StringRangeHolder(const long & new_offset, const long & new_len);
    ~StringRangeHolder() = default;
    long get_offset() const;
    long get_len() const;
    // void set_offset(long value);
    // void set_len(long value);

private:
    long offset = 0;
    long len = 0;
};

class StringHolder
{
public:
    StringHolder();
    StringHolder(const StringHolder & str);
    ~StringHolder();
    StringHolder & operator=(const StringHolder & str);
    void append_str(const char * postfix);
    void pop_back(long amount);
    long get_len() const;
    char * get_ptr() const;
    void swap_data(StringHolder & str);
    StringHolder cut(const StringRangeHolder & range);
    // void clear();

private:
    char * ptr = nullptr;
    long len = 0;
    long real_len = 0;
};

class BigIntHolder
{
public:
    BigIntHolder();
    explicit BigIntHolder(const StringHolder & temp);
    ~BigIntHolder() = default;
    void swap_data(BigIntHolder & holder);
    explicit operator StringHolder();
    bool operator>(const BigIntHolder & obj);

private:
    StringHolder str = {};
};

class ArrayRangeHolder
{
public:
    ArrayRangeHolder();
    ArrayRangeHolder(const ArrayRangeHolder & array);
    ~ArrayRangeHolder();
    ArrayRangeHolder & operator= (const ArrayRangeHolder & array);
    bool eof() const;
    void push_back(const StringRangeHolder & holder);
    // long get_size();
    // void reset();
    StringRangeHolder get_next();

private:
    StringRangeHolder * array = nullptr;
    long real_size = 0;
    long size = 0;
    long offset = 0;
};

class StringSplitter
{
public:
    StringSplitter();
    StringSplitter(const StringSplitter & splitter);
    ~StringSplitter();
    StringSplitter & operator= (const StringSplitter & splitter);
    void set_separator(const char * sep);
    ArrayRangeHolder split(const StringHolder & str);

private:
    char * separator = nullptr;
};

class FileReader
{
public:
    FileReader();
    FileReader(const FileReader & reader);
    ~FileReader();
    void set_file_name(const char * name);
    bool eof() const;
    void seek_start();
    StringHolder read_str();
    void close_fd();
    FileReader & operator= (const FileReader & reader);

private:
    int fd = -10;
    char * file_name = nullptr;
    long position = 0;

    enum { BUF_SIZE = 1024 };
};

class FileWriter
{
public:
    FileWriter();
    FileWriter(const FileWriter & writer);
    ~FileWriter();
    void set_file_name(const char * name);
    void clear();
    void write_str(const StringHolder & str);
    void close_fd();
    void seek_start();
    FileWriter & operator= (const FileWriter & writer);

private:
    int fd = -10;
    char * file_name = nullptr;
    long position = 0;

    enum { BUF_SIZE = 1024 };
};

class TempFileManager
{
public:
    TempFileManager();
    TempFileManager(const TempFileManager & mgr);
    ~TempFileManager();
    void seek_start();
    void write_str(const StringHolder & str);
    StringHolder read_str();
    bool eof();
    void close_fd();
    TempFileManager & operator=(const TempFileManager & mgr);

private:
    char * file_name = nullptr;
    FileWriter writer = {};
    FileReader reader = {};
};

int main(int argc, char **argv)
{
    try
    {
        if (argc < 2 || argc > 3)
        {
            return 1;
        }

        FileReader reader;
        FileWriter writer;
        StringSplitter splitter;
        TempFileManager tmp;

        if (argc == 3)
        {
            splitter.set_separator(argv[2]);
        }

        reader.set_file_name(argv[1]);

        while (!reader.eof())
        {
            StringHolder current = reader.read_str();

            ArrayRangeHolder array = splitter.split(current);

            BigIntHolder max_value; // default value 0

            while (!array.eof())
            {
                BigIntHolder value(current.cut(array.get_next()));

                if (value > max_value)
                {
                    max_value.swap_data(value);
                }
            }

            StringHolder str_max_value = static_cast<StringHolder>(max_value);

            tmp.write_str(str_max_value);
        }

        reader.close_fd();

        tmp.seek_start();

        writer.set_file_name(argv[1]);
        writer.clear();

        while (!tmp.eof())
        {
            StringHolder buf = tmp.read_str();
            writer.write_str(buf);
        }

        tmp.close_fd();

        writer.close_fd();

        return 0;
    }

    catch (std::exception & err)
    {
        return 1;
    }
}


FileReader::FileReader()
{
    fd = -10;
    position = 0;
    file_name = nullptr;
}

FileReader::FileReader(const FileReader & reader)
{
    fd = 0;
    position = 0;
    file_name = nullptr;
    *this = reader;
}

FileReader::~FileReader()
{
    if (fd >= 0)
    {
        close(fd);
    }

    delete [] file_name;
}

void FileReader::set_file_name(const char * name)
{
    if (file_name != nullptr)
    {
        delete [] file_name;
        file_name = nullptr;
    }

    unsigned long len = strlen(name);
    this->file_name = new char[len + 1];
    memcpy(this->file_name, name, len + 1);

    fd = open(name, O_RDONLY | O_EXCL);
    position = 0;
}

StringHolder FileReader::read_str()
{
    StringHolder str;

    char buffer[BUF_SIZE + 1];

    while (true)
    {
        long len = read(fd, buffer, BUF_SIZE);

        if (len < 0)
        {
            throw std::exception();
        }

        if (len == 0)
        {
            break;
        }

        position += len;

        buffer[len] = '\0';
        char * newline = strchr(buffer, '\n');

        long amount = len;
        if (newline != nullptr)
        {
            amount = newline - buffer + 1;
        }

        buffer[amount] = '\0';
        str.append_str(buffer);

        if (newline != nullptr)
        {
            position -= len - amount;

            lseek(fd, position, SEEK_SET);
            break;
        }
    }

    return str;
}

bool FileReader::eof() const
{
    if (fd < 0)
    {
        return true;
    }

    char buffer;
    if (0 == read(fd, &buffer, 1))
    {
        return true;
    }
    lseek(fd, -1, SEEK_CUR);
    return false;
}

void FileReader::close_fd()
{
    if (fd < 0)
    {
        return;
    }

    close(fd);
    fd = -10;
}

void FileReader::seek_start()
{
    if (fd < 0)
    {
        throw std::exception();
    }
    position = 0;
    lseek(fd, 0, SEEK_SET);
}

FileReader & FileReader::operator= (const FileReader & reader)
{
    if (&reader == this)
    {
        return *this;
    }

    fd = -10;
    position = 0;
    unsigned long len = strlen(reader.file_name);
    file_name = new char[len + 1];
    memcpy(file_name, reader.file_name, len + 1);
    return *this;
}


FileWriter::FileWriter()
{
    fd = -10;
    position = 0;
    file_name = nullptr;
}

FileWriter::FileWriter(const FileWriter & writer)
{
    fd = 0;
    position = 0;
    file_name = nullptr;
    *this = writer;
}

FileWriter & FileWriter::operator=(const FileWriter & writer)
{
    if (this == &writer)
    {
        return *this;
    }

    fd = -10;
    position = 0;
    unsigned long len = strlen(writer.file_name);
    file_name = new char[len + 1];
    memcpy(file_name, writer.file_name, len + 1);

    return *this;
}

FileWriter::~FileWriter()
{
    if (fd >= 0)
    {
        close(fd);
    }

    delete [] file_name;
}

void FileWriter::set_file_name(const char * name)
{
    if (file_name != nullptr)
    {
        delete [] file_name;
        file_name = nullptr;
    }

    unsigned long len = strlen(name);
    this->file_name = new char[len + 1];
    memcpy(this->file_name, name, len + 1);

    fd = open(name, O_WRONLY | O_CREAT, 0666);
    position = 0;
}

void FileWriter::write_str(const StringHolder & str)
{
    long len = str.get_len();
    char *ptr = str.get_ptr();

    for (long i = 0; i < len / BUF_SIZE; i++)
    {
        long code = write(fd, ptr, BUF_SIZE);
        if (code < 0)
        {
             throw std::exception();
        }
        ptr += BUF_SIZE;
        position += BUF_SIZE;
    }

    len = len - len / BUF_SIZE * BUF_SIZE;

    long code = write(fd, ptr, len);
    if (code < 0)
    {
        throw std::exception();
    }
    position += len;
}

void FileWriter::clear()
{
    close_fd();
    fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd < 0)
    {
        throw std::exception();
    }
    position = 0;
}

void FileWriter::close_fd()
{
    if (fd >= 0)
    {
        close(fd);
        fd = -10;
    }
}

void FileWriter::seek_start()
{
    if (fd < 0)
    {
        throw std::exception();
    }
    position = 0;
    lseek(fd, 0, SEEK_SET);
}

TempFileManager::TempFileManager()
{
    char buffer[] = "tmpXXXXXX";
    int fd = mkstemp(buffer);
    if (fd < 0)
    {
        throw std::exception();
    }
    file_name = new char[sizeof buffer];
    memcpy(file_name, buffer, sizeof buffer);

    writer.set_file_name(file_name);
    reader.set_file_name(file_name);

    close(fd);
}

TempFileManager::TempFileManager(const TempFileManager & mgr)
{
    file_name = nullptr;
    *this = mgr;
}

TempFileManager & TempFileManager::operator=(const TempFileManager & mgr)
{
    if (this == &mgr)
    {
        return *this;
    }

    unsigned long len = strlen(mgr.file_name);
    file_name = new char[len + 1];
    memcpy(file_name, mgr.file_name, len + 1);

    return *this;
}

TempFileManager::~TempFileManager()
{
    unlink(file_name);
    delete [] file_name;
}

StringHolder TempFileManager::read_str()
{
    return reader.read_str();
}

void TempFileManager::write_str(const StringHolder & str)
{
    writer.write_str(str);
}

void TempFileManager::close_fd()
{
    reader.close_fd();
    writer.close_fd();
}

void TempFileManager::seek_start()
{
    reader.seek_start();
    writer.seek_start();
}

bool TempFileManager::eof()
{
    return reader.eof();
}


StringHolder::StringHolder()
{
    len = 0;
    real_len = 10;
    ptr = static_cast<char *>(malloc(real_len));
    memset(ptr, '\0', real_len);
}

StringHolder::StringHolder(const StringHolder & str)
{
    *this = str;
}

StringHolder & StringHolder::operator=(const StringHolder & str)
{
    if (this == &str)
    {
        return *this;
    }

    len = str.len;
    real_len = str.real_len;
    free(ptr);
    ptr = static_cast<char *>(malloc(real_len));
    memcpy(ptr, str.ptr, real_len);

    return *this;
}

StringHolder::~StringHolder()
{
    free(ptr);
}

void StringHolder::pop_back(long amount)
{
    if (amount < 0 || amount > len)
    {
        throw std::exception();
    }

    len -= amount;
    ptr[len] = '\0';
}

char * StringHolder::get_ptr() const
{
    return ptr;
}

long StringHolder::get_len() const
{
    return len;
}

void StringHolder::swap_data(StringHolder & str)
{
    char * buf_ptr = this->ptr;
    this->ptr = str.ptr;
    str.ptr = buf_ptr;

    long buf = this->len;
    this->len = str.len;
    str.len = buf;

    buf = this->real_len;
    this->real_len = str.real_len;
    str.real_len = buf;
}

/*
void StringHolder::clear()
{
    if (ptr != nullptr)
    {
        free(ptr);
    }
    len = 0;
    real_len = 10;
    ptr = static_cast<char *>(malloc(real_len));
    memset(ptr, '\0', real_len);
}
*/

void StringHolder::append_str(const char * postfix)
{
    long app_len = static_cast<long>(strlen(postfix));

    if (app_len + len + 1 > real_len)
    {
        real_len = app_len + len + 1;
        ptr = static_cast<char *>(realloc(ptr, real_len));
    }

    memcpy(ptr + len, postfix, app_len + 1);

    len += app_len;
}

StringHolder StringHolder::cut(const StringRangeHolder & range)
{
    long offset = range.get_offset();
    long length = range.get_len();

    if (offset < 0 || length < 0 || offset + length > len)
    {
        throw std::exception();
    }

    StringHolder holder;

    long end = offset + length;

    char save = ptr[end];
    ptr[end] = '\0';

    holder.append_str(ptr + offset);

    ptr[end] = save;

    return holder;
}

StringRangeHolder::StringRangeHolder()
{
    offset = 0;
    len = 0;
}

long StringRangeHolder::get_len() const
{
    return len;
}

long StringRangeHolder::get_offset() const
{
    return offset;
}

StringRangeHolder::StringRangeHolder(const long & new_offset, const long & new_len)
{
    this->offset = new_offset;
    this->len = new_len;
}

/*
void StringRangeHolder::set_len(long value)
{
    len = value;
}

void StringRangeHolder::set_offset(long value)
{
    offset = value;
}
*/

BigIntHolder::BigIntHolder()
{
    str.append_str("0");
}

BigIntHolder::BigIntHolder(const StringHolder & temp)
{
    this->str = temp;

    char * ptr = str.get_ptr();
    long len = str.get_len();

    if (len == 0)
    {
        throw std::exception();
    }

    if (ptr[len - 1] == '\n')
    {
        str.pop_back(1);
        len--;
    }

    if (len == 0)
    {
        throw std::exception();
    }

    long counter = 0;

    while (ptr[counter] == '0')
    {
        counter++;
    }

    if (counter > 0 && len != 1)
    {
        throw std::exception();
    }

    for (long i = 0; i < len; i++)
    {
        if (!isdigit(ptr[i]))
        {
            throw std::exception();
        }
    }
}

void BigIntHolder::swap_data(BigIntHolder & holder)
{
    this->str.swap_data(holder.str);
}

BigIntHolder::operator StringHolder()
{
    StringHolder buf = str;
    buf.append_str("\n");
    return buf;
}

bool BigIntHolder::operator>(const BigIntHolder & obj)
{
    long my_len = this->str.get_len();
    long obj_len = obj.str.get_len();

    if (my_len > obj_len) return true;
    if (my_len < obj_len) return false;

    return strcmp(this->str.get_ptr(), obj.str.get_ptr()) > 0;
}

ArrayRangeHolder::ArrayRangeHolder()
{
    real_size = 10;
    size = 0;
    offset = 0;
    array = new StringRangeHolder[real_size];
}

ArrayRangeHolder::ArrayRangeHolder(const ArrayRangeHolder & arr)
{
    *this = arr;
}

ArrayRangeHolder & ArrayRangeHolder::operator=(const ArrayRangeHolder & arr)
{
    if (this == &arr)
    {
        return *this;
    }

    this->real_size = arr.real_size;
    this->size = arr.size;
    this->offset = arr.offset;

    delete [] array;
    this->array = new StringRangeHolder[this->real_size];
    memcpy(this->array, arr.array, (sizeof *array) * size);

    return *this;
}

ArrayRangeHolder::~ArrayRangeHolder()
{
    delete [] array;
}

void ArrayRangeHolder::push_back(const StringRangeHolder & holder)
{
    if (array == nullptr)
    {
        real_size = 10;
        size = 0;
        offset = 0;
        array = new StringRangeHolder[real_size];
    }

    if (size >= real_size)
    {
        real_size *= 2;
        auto new_array = new StringRangeHolder[real_size];
        memcpy(new_array, array, (sizeof *array) * size);
        delete [] array;
        array = new_array;
    }

    array[size++] = holder;
}

StringRangeHolder ArrayRangeHolder::get_next()
{
    if (eof())
    {
        throw std::exception();
    }

    if (array == nullptr)
    {
        throw std::exception();
    }

    return array[offset++];
}

bool ArrayRangeHolder::eof() const
{
    if (offset == size)
    {
        return true;
    }

    if (offset < 0 || offset > size)
    {
        throw std::exception();
    }

    return false;
}

StringSplitter::StringSplitter()
{
    separator = nullptr;
}

StringSplitter::StringSplitter(const StringSplitter & splitter)
{
    *this = splitter;
}

StringSplitter & StringSplitter::operator=(const StringSplitter & splitter)
{
    if (this == &splitter)
    {
        return *this;
    }

    unsigned long len = strlen(splitter.separator);

    delete [] separator;
    separator = new char[len + 1];
    memcpy(separator, splitter.separator, len + 1);

    return *this;
}

StringSplitter::~StringSplitter()
{
    delete [] separator;
}

void StringSplitter::set_separator(const char * sep)
{
    delete [] separator;
    unsigned long len = strlen(sep);
    separator = new char[len + 1];
    memcpy(separator, sep, len + 1);
}

ArrayRangeHolder StringSplitter::split(const StringHolder & str)
{
    ArrayRangeHolder array;

    long cur_offset = 0;

    char * ptr = str.get_ptr();

    long sep_len = static_cast<long>(strlen(separator));

    while (true)
    {
        char * search = strstr(ptr + cur_offset, separator);

        long len;

        if (search == nullptr)
        {
            len = str.get_len() - cur_offset;
        }
        else
        {
            len = search - ptr - cur_offset;
        }

        array.push_back(StringRangeHolder(cur_offset, len));

        cur_offset += len + sep_len;

        if (search == nullptr)
        {
            break;
        }
    }

    return array;
}
