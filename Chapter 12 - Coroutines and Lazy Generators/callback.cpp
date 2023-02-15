#include <concepts>
#include <iostream>

template <typename T, typename F>
requires std::invocable<F&, const T&>
void lin_space(T start, T stop, std::size_t n, F &&f) {
    T diff = static_cast<T>(stop - start) / (n - 1);
    for (int i = 0; i != n; ++i) {
        f(start + diff * i);
    }
}

int main()
{
    lin_space(2.0f, 3.0f, 5, [] (auto v) { std::cout << v << ' '; } );
    std::cout << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 2 2.25 2.5 2.75 3
// Program ended with exit code: 0
