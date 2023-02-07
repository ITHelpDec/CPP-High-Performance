#include <iostream>
#include <future>

auto divide(int x, int y) {
    if (!y) { throw std::runtime_error("Oops! Trying to divide by zero..."); }
    return x / float(y);
}

int main()
{
    try {
        std::cout << "Result: " << std::async(divide, 45, 5).get() << '\n';
    } catch (const std::exception &e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
    
    return 0;
}
