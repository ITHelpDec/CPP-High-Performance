#include <array>
#include <random>
#include <vector>
#include <chrono>
#include <iostream>

struct SmallObject {
    std::array<char, 4> data_;
    int score_ = gen();
    
    int gen() {
        static std::default_random_engine e;
        static std::uniform_int_distribution u(0, 10);
        return u(e);
    }
};

struct LargeObject {
    std::array<char, 256> data_;
    int score_ = gen();
    
    int gen() {
        static std::default_random_engine e;
        static std::uniform_int_distribution u(0, 10);
        return u(e);
    }
};

int main()
{
    std::vector<SmallObject> small_objects(1000000);
    std::vector<LargeObject> large_objects(1000000);
    int small_sum, large_sum;
    
    std::cout << "SmallObjects size: " << sizeof(SmallObject) << "\n";
    std::cout << "LargeObjects size: " << sizeof(LargeObject) << "\n\n";
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // test for slow first timer
    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Testing...\n";
    auto t2 = std::chrono::high_resolution_clock::now();
    auto test = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    small_sum = std::accumulate(small_objects.begin(), small_objects.end(), 0,
                                [] (const int &n, const SmallObject &so) { return n + so.score_; } );
    t2 = std::chrono::high_resolution_clock::now();
    auto result1 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    large_sum = std::accumulate(large_objects.begin(), large_objects.end(), 0,
                                [] (const int &n, const LargeObject &lo) { return n + lo.score_; } );
    t2 = std::chrono::high_resolution_clock::now();
    auto result2 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::cout << "small: " << result1 << " ns (sum = " << small_sum << ")\n";
    std::cout << "large: " << result2 << " ns (sum = " << large_sum << ")\n";
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    small_sum = 0, large_sum = 0;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    large_sum = std::accumulate(large_objects.begin(), large_objects.end(), 0,
                                [] (const int &n, const LargeObject &lo) { return n + lo.score_; } );
    t2 = std::chrono::high_resolution_clock::now();
    auto result3 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    small_sum = std::accumulate(small_objects.begin(), small_objects.end(), 0,
                                [] (const int &n, const SmallObject &so) { return n + so.score_; } );
    t2 = std::chrono::high_resolution_clock::now();
    auto result4 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::cout << "small: " << result4 << " ns (sum = " << small_sum << ")\n";
    std::cout << "large: " << result3 << " ns (sum = " << large_sum << ")\n\n";
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// SmallObjects size: 8
// LargeObjects size: 260

// Testing...
// small: 14378111 ns (sum = 5001712)
// large: 29432939 ns (sum = 5001712)
// small: 13872357 ns (sum = 5001712)
// large: 28542771 ns (sum = 5001712)

// Program ended with exit code: 0
