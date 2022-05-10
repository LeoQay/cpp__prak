#include <iostream>
#include <vector>

int main()
{
    std::vector<int> seq;

    int current;
    while(std::cin >> current)
    {
        seq.push_back(current);
    }

    if (!seq.empty())
    {
        int count = 0, last = seq.back();
        for (int it: seq)
        {
            count += it > last ? 1 : 0;
        }
        std::cout << count << std::endl;
    }

    return 0;
}

