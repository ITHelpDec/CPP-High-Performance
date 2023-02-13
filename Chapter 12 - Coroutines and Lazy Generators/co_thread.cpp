#include <iostream>
#include <thread>
#include <chrono>

Resumable coroutine() {
    std::cout << "1...";
    co_await std::experimental::suspend_always();
    std::cout << "2 \n";
}

Resumable coro_factory() {
    Resumable res = coroutine();
    return res;
}

int main()
{
    Resumable r = coroutine();
    r.resume();
    
    std::thread t( [ r = std::move(r) ] () mutable {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        r.resume();
    } );
    
    t.join();
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 1...2 
// Program ended with exit code: 0
