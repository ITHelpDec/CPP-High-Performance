#include <iostream>
#include <future>
#include <thread>

auto divide(int x, int y) {
    if (!y) { throw std::runtime_error("Oops! Trying to divide by zero..."); }
    return x / float(y);
} // tweaked slightly from book i.e. 45 / 8 ≠ 5

int main()
{
    std::packaged_task<decltype(divide)> task(divide);
    
    // NB: exception will be throw if get_future() is not called ahead of thread
    auto f = task.get_future();
 
    std::thread(std::move(task), 45, 5).detach();
    
    try {
        // std::cout << "Result: " << task.get_future().get() << '\n';
        std::cout << "Result: " << f.get() << '\n';
    } catch (const std::exception &e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Result: 9
// Program ended with exit code: 0
