#include <vector>
#include <ranges>
#include <algorithm>
#include <list>
#include <iostream>

auto to_vector(auto &&r) {
    std::vector<std::ranges::range_value_t<decltype(r)>> v;
    
    // basically, if r has a guaranteed size at compile time
    if constexpr(std::ranges::sized_range<decltype(r)>) {
        v.reserve(std::ranges::size(r));
    }
    
    std::ranges::copy(r, std::back_inserter(v));
    
    return v;
}

template <typename T>
void printContainer(const T& t) {
    for (const auto &e : t) {
        std::cout << e << " ";
    } std::cout << std::endl;
}

int main()
{
    std::list<int> ilist = { 1, 2,3 , 4, 5 };
    std::cout << "ilist: "; printContainer(ilist);

    auto v = to_vector(ilist);
    std::cout << "v    : "; printContainer(v);

    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ilist: 1 2 3 4 5 
v    : 1 2 3 4 5 
