#include <iostream>

bool check(std::string & str)
{
    int count = 0;
    size_t pos = 0, len = str.size();

    while (pos + 2 <= len && str[pos] == 'a' && str[pos + 1] == 'b')
    {
        count++;
        pos += 2;
    }

    if (count == 0)
    {
        return str.empty();
    }

    while (pos < len && str[pos] == 'c')
    {
        pos++;
        count--;

        if (count < 0)
        {
            return false;
        }
    }

    return count == 0 && pos == len;
}

int main()
{
    std::cout << 0 << std::endl;
    std::cout << 2 << std::endl;

    std::string str;

    while (getline(std::cin, str))
    {
        if (check(str))
        {
            std::cout << "YES" << std::endl;
        }
        else
        {
            std::cout << "NO" << std::endl;
        }
    }

    return 0;
}
