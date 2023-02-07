#include <future>
#include <thread>
#include <iostream>

// why use auto for void function?
void divide(int x, int y, std::promise<int> &p) {
    if (y == 0) {
        // make exception then pass as pointer
        std::runtime_error e("Divide by 0 error.");
        p.set_exception(std::make_exception_ptr(e));
    } else {
        p.set_value(x / y);
    }
}

int main()
{
    std::promise<int> p;
    
    // need to use std::ref; "&" will not work
    std::thread(divide,45, 5, std::ref(p)).detach();
    
    try { // we use .get() to call the actual value
        std::cout << "Result: " << p.get_future().get() << '\n';
    } catch (const std::exception &e) {
        std::cout << "\nCaught exception!!! - " << e.what() << '\n';
    }
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Result: 9
// Program ended with exit code: 0
// or
// Result: Caught exception!!! - Divide by 0 error.
// Program ended with exit code: 0
