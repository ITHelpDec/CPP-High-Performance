#include <set>
#include <string>
#include <chrono>
#include <iostream>

int main()
{
    std::multiset<std::string> mset = { "man", "bear", "pig", "woof", "fiddly", "dee" };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != 100; /*...*/) {
        if (mset.count("woof")) {
            ++i;
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto test = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != 100; /*...*/) {
        if (mset.find("woof") != mset.end()) {
            ++i;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    auto result2 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != 100; /*...*/) {
        if (mset.lower_bound("woof") != mset.end()) {
            ++i;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    auto result3 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != 100; /*...*/) {
        if (mset.contains("woof")) {
            ++i;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    auto result4 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != 100; /*...*/) {
        if (mset.count("woof")) {
            ++i;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    auto result5 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::cout << "test:        " << test    << " ns\n";
    std::cout << "find:        " << result2 << " ns\n";
    std::cout << "lower_bound: " << result3 << " ns\n";
    std::cout << "contains:    " << result4 << " ns\n";
    std::cout << "count:       " << result5 << " ns" << std::endl;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// test:        68493 ns
// find:        48667 ns
// lower_bound: 37479 ns
// contains:    50740 ns
// count:       67762 ns
// Program ended with exit code: 0
