#include <string_view>
#include <chrono>
#include <iostream>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::string s = "manbearpig";
    auto t2 = std::chrono::high_resolution_clock::now();
    auto s_result = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    t1 = std::chrono::high_resolution_clock::now();
    std::string_view sv = "manbearpig";
    t2 = std::chrono::high_resolution_clock::now();
    auto sv_result = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    t1 = std::chrono::high_resolution_clock::now();
    std::string_view sv2 = "howdoishotweb?";
    t2 = std::chrono::high_resolution_clock::now();
    auto sv_result2 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    t1 = std::chrono::high_resolution_clock::now();
    std::string s2 = "howdoishotweb?";
    t2 = std::chrono::high_resolution_clock::now();
    auto s_result2 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    t1 = std::chrono::high_resolution_clock::now();
    std::string s3 = "i like-a...do...da \"cha-cha\"...";
    t2 = std::chrono::high_resolution_clock::now();
    auto s_result3 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    t1 = std::chrono::high_resolution_clock::now();
    std::string_view sv3 = "i like-a...do...da \"cha-cha\"...";
    t2 = std::chrono::high_resolution_clock::now();
    auto sv_result3 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    std::cout << "std::string     : " << s_result   << " ns\n";
    std::cout << "std::string_view: " << sv_result  << " ns\n";
    std::cout << "std::string_view: " << sv_result2 << " ns\n";
    std::cout << "std::string     : " << s_result2   << " ns\n";
    std::cout << "std::string     : " << s_result3   << " ns\n";
    std::cout << "std::string_view: " << sv_result3  << " ns\n";

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// chrono high resolution clocks are always slow on the first benchmark...
// ..., but actually – std::string_view is very fast, especially with more complex strings

// std::string     : 2909 ns
// std::string_view: 94 ns
// std::string_view: 76 ns
// std::string     : 108 ns
// std::string     : 5015 ns
// std::string_view: 117 ns
// Program ended with exit code: 0
