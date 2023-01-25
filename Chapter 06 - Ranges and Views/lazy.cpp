#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>

auto to_vector(auto &&r) {
    std::vector<std::ranges::range_value_t<decltype(r)>> v;

    if constexpr(std::ranges::sized_range<decltype(r)>) {
        v.reserve(std::ranges::size(r));
    }

    std::ranges::copy(r, std::back_inserter(v));

    return v;
}

void printVec(const std::vector<int> &ivec) {
    for (const auto &e : ivec) {
        std::cout << e << " ";
    } std::cout << std::endl;
}

int main()
{
    std::vector<int> ivec = { 1, 2, 3, 4, 5 };
    std::cout << "ivec       : "; printVec(ivec);

    auto odd_numbers = ivec
        | std::views::filter( [] (auto i) { return i % 2; } );
        
    // does not compile
    // std::ranges::sort(odd_numbers);

    // materialise, then sort
    std::vector<int> odd_vec = to_vector(odd_numbers);
    std::ranges::sort(odd_vec);

    std::cout << "ivec (odds): "; printVec(odd_vec);

    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// ivec       : 1 2 3 4 5 
// ivec (odds): 1 3 5 
