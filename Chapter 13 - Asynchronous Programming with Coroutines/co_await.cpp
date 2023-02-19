template <class Rep, class Period>
auto operator co_await(std::chrono::duration<Rep, Period> d) {
    
    struct Awaitable {
        std::chrono::system_clock::duration d_;
        Awaitable(std::chrono::system_clock::duration d) : d_(d) { }
        
        bool await_ready() const { return d.count() <= 0; }
        bool await_suspend(std::experimental::coroutine_handle<> h) { }
        bool await_resume() { }
    };
    
    return Awaitable(d);
}

// great and all, but requires a lot of boilerplate to get it to actually do something meaningful like in the author's example

// can't stick this directly into main()
// std::cout << "just about to go to sleep...\n";
// co_await 10ms;       // Calls operator co_await()
// std::cout << "resumed\n";
