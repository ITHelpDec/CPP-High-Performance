auto func(std::mutex &m, bool x, bool y) {
    std::scoped_lock guard(m);
    if (x) {
        // guard releases mutex at early exit
        return;
    }
    
    if (y) {
        // guard releases mutex if exception is thrown
        throw std::exception();
    }
} // guard also releases mutex at function exit
