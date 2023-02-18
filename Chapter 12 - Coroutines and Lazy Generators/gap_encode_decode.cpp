#include "../../Source Code/Chapter12/generator.h"
#include <vector>
#include <iostream>

template <typename Range>
Generator<int> gap_encode(Range &ids) {
    // bit of C++17 "init if" for pure shiggles
    for (int last_id = 0; const auto &id : ids) {
        const int gap = id - last_id;
        last_id = id;
        co_yield gap;
    }
}

template <typename Range>
Generator<int> gap_decode(Range &gaps) {
    // bit of C++17 "init if" for pure shiggles
    for (int last_id = 0; const auto &gap : gaps) {
        const int id = gap + last_id;
        co_yield id;
        last_id = id;
    }
}

void printVec(const std::vector<int> &ivec) {
    for (const auto &e : ivec) { std::cout << e << ' '; }
}

int main()
{
    std::vector<int> ids = { 1, 12, 13, 24, 95 };
    std::cout << "ivec: "; printVec(ids); std::cout << " (original)\n";
    
    for (int i = 0; auto &&gap : gap_encode(ids)) { ids[i++] = gap; }
    std::cout << "ivec: "; printVec(ids); std::cout << "  (encoded)\n";
    
    for (int i = 0; auto &&gap : gap_decode(ids)) { ids[i++] = gap; }
    std::cout << "ivec: "; printVec(ids); std::cout << " (decoded)\n";
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// ivec: 1 12 13 24 95  (original)
// ivec: 1 11 1 11 71   (encoded)
// ivec: 1 12 13 24 95  (decoded)
// Program ended with exit code: 0
