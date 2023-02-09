#include <atomic>
#include <iostream>
#include <random>
#include <thread>
#include <cassert>

struct Coins {
    // int heads_ = 0, tails_ = 0;
    std::atomic<int> heads_ = 0, tails_ = 0;
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
    auto flip = [&flips] (int start, int end) {
        for (int i = start; i != end; ++i) {
            // no need for atomic_ref (code is cleaner too)
            lands_heads() ? ++flips.heads_ : ++flips.tails_;
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
    assert((flips.heads_ + flips.tails_) == 100);
    
    return 0;
}
