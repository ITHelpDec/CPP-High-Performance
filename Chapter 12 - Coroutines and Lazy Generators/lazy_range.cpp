#include <iostream>
#include <cassert>
#include <ranges>
#include <cmath>

template <typename T>
auto lin_value(T start, T stop, size_t index, size_t n) {
    assert(n > 1 && index < n);
    const auto amount = static_cast<T>(index) / (n - 1);
    const auto v = std::lerp(start, stop, amount); // C++20
    return v;
}

template <typename T>
auto lin_space(T start, T stop, std::size_t n) {
    return std::views::iota(std::size_t{0}, n)
            | std::views::transform( [=] (auto i) {
                return lin_value(start, stop, i, n);
            } );
}

int main()
{
    for (auto v : lin_space(2.0f, 3.0f, 5)) {
        std::cout << v << ' ';
    } std::cout << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// https://godbolt.org/z/5c7WssPhW (again, only works on GCC)
// Program returned: 0
// Program stdout
// 2 2.25 2.5 2.75 3 
