// #include "generator.h"
#include <iostream>
#include <cassert>
#include <cmath>

template <typename T>
auto lin_value(T start, T stop, size_t index, size_t n) {
    assert(n > 1 && index < n);
    const auto amount = static_cast<T>(index) / (n - 1);
    const auto v = std::lerp(start, stop, amount); // C++20
    return v;
}

template <typename T>
Generator<T> lin_space(T start, T stop, std::size_t n) {
    for (int i = 0; i != n; ++i) {
        co_yield lin_value(start, stop, i, n);
    }
}

int main()
{
    for (auto &&v : lin_space(2.0f, 3.0f, 5)) {
        std::cout << v << ' ';
    } std::cout << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 2 2.25 2.5 2.75 3 
// Program ended with exit code: 0
