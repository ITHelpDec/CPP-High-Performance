// #include "resumable.h"
#include <iostream>

struct Widget {
    int x_ = 67;
    
    Resumable cooroutine()
    {
        std::cout << ++x_ << ' ';
        co_await std::experimental::suspend_always();
        std::cout << ++x_ << " (nice...)\n";
    }
};

int main()
{
    Widget widget;
    
    // does not keep object alive
    Resumable coro = widget.cooroutine();
    
    // call x2
    coro.resume();
    coro.resume();
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 68 69 (nice...)
// Program ended with exit code: 0
