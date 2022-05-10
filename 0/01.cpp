class StringHolder
{
public:
    StringHolder();
    // StringHolder(const char * str);
    ~StringHolder();
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

class FileReader
{
public:
    FileReader();
    ~FileReader();
    void set_file_name(const char * file_name);
    // void set_descriptor(int fd);
    bool eof();
    // void reset();
    StringHolder read_str();
    void close();
};

class FileWriter
{
public:
    FileWriter();
    ~FileWriter();
    void set_file_name(const char * file_name);
    void clear();
    // void set_descriptor(int fd);
    void write_str(const StringHolder & str);
    void close_fd();
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

class TempFileManager
{
public:
    TempFileManager();
    ~TempFileManager();
    void seek_start();
    void write_str(const StringHolder & str);
    // StringHolder & read();
    StringHolder read(long limit);
    bool eof();
    void close_fd();
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

    reader.close();

    tmp.seek_start();

    writer.set_file_name(argv[1]);
    writer.clear();

    while (!tmp.eof())
    {
        StringHolder buf = tmp.read(1024);
        writer.write_str(buf);
    }

    tmp.close_fd();

    writer.close_fd();

    return 0;
}
