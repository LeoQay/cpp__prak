#include <set>
#include <map>
#include <string>
#include <unordered_set>

void
generate_inside(
        std::unordered_set<std::string> & passed,
        std::set<std::string> & result,
        const std::multimap<std::string, std::string> & dictionary,
        const std::string & current,
        size_t max_len)
{
    if (current.size() > max_len)
    {
        return;
    }

    if (passed.find(current) != passed.end())
    {
        return;
    }

    passed.insert(current);

    int count = 0;

    for (auto & it : dictionary)
    {
        size_t pos = 0;
        while (true)
        {
            size_t offset = current.substr(pos).find(it.first);
            if (static_cast<int>(offset) == -1)
            {
                break;
            }
            pos += offset;
            count++;

            size_t len = it.first.size();
            std::string next = current.substr(0, pos);
            next += it.second;
            next += current.substr(pos + len);

            generate_inside(passed, result, dictionary, next, max_len);

            pos++;
        }
    }

    if (count == 0)
    {
        result.insert(current);
    }
}


std::set<std::string>
generate(
        const std::multimap<std::string, std::string> & g,
        const std::string & start,
        size_t max_len)
{
    for (auto & it : g)
    {
        if (it.first.empty() || it.first.size() > it.second.size())
        {
            throw std::exception();
        }
    }

    std::set<std::string> result;
    std::unordered_set<std::string> passed;

    generate_inside(passed, result, g, start, max_len);

    return result;
}


/*
typedef std::pair<std::string, std::string> str_pair;

int main()
{
    std::multimap<std::string, std::string> m;

    m.insert(str_pair("ta", "qty"));

    auto ret = generate(m, "starta", 50);

    return 0;
}
*/
