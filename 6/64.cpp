#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

int main()
{
    std::vector<int> v;

    std::istream_iterator<int> input_int(std::cin);
    std::istream_iterator<int> end;

    std::copy(input_int, end, std::back_inserter(v));

    if (v.empty())
    {
        return 0;
    }

    std::unordered_map<int, int> unique_stat;

    std::for_each(v.begin(), v.end(), [&unique_stat] (int elem)
    { unique_stat[elem]++; });

    std::pair<int, int> best = *unique_stat.begin();

    std::for_each(unique_stat.begin(), unique_stat.end(),
                  [&best] (std::pair<int, int> it)
                  { if (it.second > best.second) best = it; } );

    std::unordered_set<int> best_elems;

    std::for_each(unique_stat.begin(), unique_stat.end(),
                  [&best_elems, best] (std::pair<int, int> it)
                  { if (it.second == best.second)
                  {best_elems.insert(it.first); } });

    std::transform(v.begin(), v.end(), v.begin(), [&best_elems] (int elem)
    {
        if (best_elems.find(elem) != best_elems.end())
            {return -elem; } else {return elem;}
    });

    std::ostream_iterator<int> output_int(std::cout, "\n");

    std::copy(v.rbegin(), v.rend(), output_int);

    return 0;
}
