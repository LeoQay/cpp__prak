#include <map>
#include <string>
#include <algorithm>

bool increased(const std::map<std::string, std::string> & container)
{
    return std::all_of(
            container.begin(), container.end(),
            [] (const std::pair<std::string, std::string> & p) {
                return static_cast<int>(p.second.find(p.first)) != -1;
    }
    );
}


/*
#include <iostream>

int main()
{
    std::map<std::string, std::string> m;

    m["key"] = "keys";

    m["prefix"] = "prefixed";

    std::cout << increased(m);

    return 0;
}
*/
