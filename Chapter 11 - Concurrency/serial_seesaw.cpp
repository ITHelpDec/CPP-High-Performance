#include <iostream>

void increase(std::size_t &x) {
    for (int i = 0; i != 100; ++i) { ++x; }
}

void decrease(std::size_t &x) {
    for (int i = 0; i != 100; ++i) { --x; }
}

int main()
{
    std::size_t x = 0;

    increase(x);
    decrease(x);

    // x will be 0
    std::cout << x << '\n';

    return 0;
}
