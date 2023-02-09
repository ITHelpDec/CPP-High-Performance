#include <iostream>
#include <random>
#include <thread>
#include <atomic>
#include <assert>

struct Coins {
    int heads_ = 0, tails_ = 0;
};

std::ostream& operator<<(std::ostream &os, Coins &flips) {
    os << "heads: " << flips.heads_ << ", tails: " << flips.tails_;
    return os;
}

bool lands_heads() {
    static thread_local std::default_random_engine e;
    static std::bernoulli_distribution u;
    return u(e);
}

void flip_mt(std::size_t n, Coins &flips)
{
    // i tweaked the example from the book slightly
    // it looks like the author is just going from 0 to n/2 in each thread...
    // i'd have thought that if you wanted to do "first half" and "second half" of 100...
    // ...then you would define a start and an end, especially if indices in containers are important
    auto flip = [&flips] (int start, int end) {
        for (int i = start; i != end; ++i) {
            lands_heads() ? ++std::atomic_ref<int>(flips.heads_)
                          : ++std::atomic_ref<int>(flips.tails_);
            std::cout << flips << '\n';
        }
    };
    
    std::thread t1(flip, 0, n/2);
    std::thread t2(flip, n/2, n);
    
    t1.join();
    t2.join();
}

int main()
{
    Coins flips;
    flip_mt(100, flips);
    assert( (flips.heads_ + flips.tails_) == 100);
    
    return 0;
}
