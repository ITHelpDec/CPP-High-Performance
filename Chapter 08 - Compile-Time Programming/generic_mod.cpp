#include <cassert>
#include <type_traits>
#include <cmath>
#include <iostream>

template <typename T, typename U>
auto generic_mod(const T &t, const U &u) -> decltype(t / u) {
    assert(u != 0);
    if constexpr (std::is_floating_point_v<decltype(t / u)>) { return std::fmod(t, u); }
    else { return t % u; }
}

int main()
{
    std::cout << generic_mod(3, 2) << '\n';
    
    std::cout << generic_mod(3.5f, 2) << '\n';
    
    std::cout << generic_mod(2, 1.5) << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 1
// 1.5
// 0.5
// Program ended with exit code: 0
