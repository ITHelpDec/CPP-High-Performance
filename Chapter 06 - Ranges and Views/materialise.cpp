#include <list>
#include <string>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
void printContainer(const T &t) {
    for (const auto &e : t) {
        std::cout << e << " ";
    } std::cout << std::endl;
}

int main()
{
    std::list<int> ints = { 2, 3, 4, 2, 1 };
    std::cout << "ints                   : "; printContainer(ints);


    auto r = ints
        | std::views::transform([](auto i) { return std::to_string(i); });

    auto vec = std::vector<std::string>{};
    std::ranges::copy(r, std::back_inserter(vec));
    std::cout << "ints (views and ranges): "; printContainer(vec);

    std::list<std::string> slist;
    std::transform(ints.begin(), ints.end(), std::back_inserter(slist), [] (auto i) { return std::to_string(i); } );
    std::cout << "ints (transformed)     : "; printContainer(slist);

    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// ints                   : 2 3 4 2 1 
// ints (views and ranges): 2 3 4 2 1 
// ints (transformed)     : 2 3 4 2 1 
