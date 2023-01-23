#include <vector>
#include <random>
#include <iostream>
#include <chrono>

class Gen {
public:
    Gen(std::size_t n)
    {
        ivec_.resize(n);
        std::generate(ivec_.begin(), ivec_.end(), [this] () { return random_int(); } );
    }

    std::vector<int> ivec_;
private:
    int random_int()
    {
        static std::default_random_engine e;
        static std::uniform_int_distribution u(-100, 100);
        return u(e);
    }
};

void printVec(const std::vector<int> &ivec) {
    for (const auto &e : ivec) {
        std::cout << e << " ";
    } std::cout << std::endl;
}

void bm_sort(int n) {
    for (int i = 0; i != n; ++i) {
        std::vector<int> ivec = Gen(n).ivec_;
        std::sort(ivec.begin(), ivec.end());
    }
}

void bm_nth_element(int n) {
    for (int i = 0; i != n; ++i) {
        std::vector<int> ivec = Gen(n).ivec_;
        std::nth_element(ivec.begin(), ivec.begin() + ivec.size() / 2, ivec.end());
    }
}

void bm_partial_sort(int n) {
    for (int i = 0; i != n; ++i) {
        std::vector<int> ivec = Gen(n).ivec_;
        std::partial_sort(ivec.begin(), ivec.begin() + ivec.size() / 10, ivec.end());
    }
}

int main()
{
    int n = 5000;
    std::vector<int> testcase = Gen(n).ivec_;
    
    // to skip the chrono glitch
    auto t1 = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto test = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    bm_sort(n);
    t2 = std::chrono::high_resolution_clock::now();
    auto bm_sort_result = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    bm_nth_element(n);
    t2 = std::chrono::high_resolution_clock::now();
    auto bm_nth_result = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    bm_partial_sort(n);
    t2 = std::chrono::high_resolution_clock::now();
    auto bm_partial_result = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::cout << "bm_sort:          " << bm_sort_result    << " ms\n";
    std::cout << "bm_nth_element:   " << bm_nth_result     << " ms\n";
    std::cout << "bm_partial_sort:  " << bm_partial_result << " ms\n\n";
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// bm_sort:          1756 ms
// bm_nth_element:   1249 ms
// bm_partial_sort:  1421 ms

// Program ended with exit code: 0
