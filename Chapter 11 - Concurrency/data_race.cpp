#include <iostream>
#include <thread>

void increase(std::size_t &x) {
    for (int i = 0; i != 100; ++i) { ++x; }
}

void decrease(std::size_t &x) {
    for (int i = 0; i != 100; ++i) { --x; }
}

int main()
{
    std::size_t x = 0;

    std::thread t1 = [&] () { increase(x); }
    std::thread t2 = [&] () { decrease(x); }

    t1.join();
    t2.join();

    // x could be 18446744073709251125
    std::cout << x << '\n';

    return 0;
}
