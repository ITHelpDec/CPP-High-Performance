#include <iostream>
#include <random>

struct Coins {
    int heads_ = 0, tails_ = 0;
};

std::ostream& operator<<(std::ostream &os, Coins &flips) {
    os << "heads: " << flips.heads_ << ", tails: " << flips.tails_;
    return os;
}

bool lands_heads() {
    static std::default_random_engine e;
    static std::bernoulli_distribution u;
    return u(e);
}

void flip(std::size_t n, Coins &flips) {
    for (int i = 0; i != n; ++i) {
        lands_heads() ? ++flips.heads_ : ++flips.tails_;
        std::cout << flips << '\n';
    }
}

int main()
{
    Coins flips;
    flip(100, flips);
    assert((flips.heads_ + flips.tails_) == 100);
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// heads: 1, tails: 0
// heads: 1, tails: 1
// heads: 2, tails: 1
// ...
// heads: 45, tails: 53
// heads: 46, tails: 53
// heads: 47, tails: 53
// Program ended with exit code: 0
