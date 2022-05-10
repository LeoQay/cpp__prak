#include <iostream>
#include <list>

int main()
{
    std::list<std::pair<char, int>> stat;

    std::string str;
    getline(std::cin, str);

    stat.push_back({str[0], 1});

    size_t len = str.length();
    for (size_t i = 1; i < len; i++)
    {
        auto & ref = stat.back();

        if (ref.first == str[i])
        {
            ref.second++;
        }
        else
        {
            stat.push_back({str[i], 1});
        }
    }

    

    return 0;
}
