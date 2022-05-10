#include <cstring>
#include "unistd.h"
#include "fcntl.h"
#include <cstdlib>

class StringHolder
{
public:
    StringHolder();
    // StringHolder(const char * str);
    ~StringHolder();

    void append_str(char * postfix);
    long get_len() const;
    char * get_ptr() const;
};

class StringRangeHolder
{
public:
    StringRangeHolder();
    ~StringRangeHolder();
};

class BigIntHolder
{
public:
    BigIntHolder();
    explicit BigIntHolder(long value);
    BigIntHolder(const StringHolder & str, const StringRangeHolder & range);
    ~BigIntHolder();
    void swap_data(BigIntHolder & holder);
    explicit operator StringHolder();
};

class BigIntComp
{
public:
    // static bool equal(const BigIntHolder & a, const BigIntHolder & b);
    static bool large(const BigIntHolder & a, const BigIntHolder & b);
    // static bool less(const BigIntHolder & a, const BigIntHolder & b);
};

class ArrayRangeHolder
{
public:
    ArrayRangeHolder();
    ~ArrayRangeHolder();
    bool eof();
    // long get_size();
    // void reset();
    StringRangeHolder get_next();
};

class StringSplitter
{
public:
    StringSplitter();
    ~StringSplitter();
    void set_separator(const char * separator);
    void set_string(const StringHolder & string);
    ArrayRangeHolder split();
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

        splitter.set_string(current);

        ArrayRangeHolder array = splitter.split();

        BigIntHolder max_value(0);

        while (!array.eof())
        {
            BigIntHolder value(current, array.get_next());

            if (BigIntComp::large(value, max_value))
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
            throw;
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
            position -= len - (newline - buffer) - 1;

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
    if (fd < 0) {
        return;
    }

    close(fd);
    fd = -10;
}

void FileReader::seek_start()
{
    if (fd < 0)
    {
        throw;
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
            throw;
        }
        ptr += BUF_SIZE;
        position += BUF_SIZE;
    }

    len = len - len / BUF_SIZE * BUF_SIZE;

    long code = write(fd, ptr, len);
    if (code < 0)
    {
        throw;
    }
    position += len;
}

void FileWriter::clear()
{
    close_fd();
    fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd < 0)
    {
        throw;
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
        throw;
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
        throw;
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
