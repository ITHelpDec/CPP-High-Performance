#include <iostream>
#include <future>
#include <vector>

namespace par {

// guts of the operation
template <class _ForwardIt, class _UnaryOperation>
std::size_t count_if(_ForwardIt __first, _ForwardIt __last, _UnaryOperation __op, std::size_t chunk) {
    std::size_t n = static_cast<std::size_t>(std::distance(__first, __last));
    
    if (n <= chunk) { return std::count_if(__first, __last, __op); }
    
    _ForwardIt __middle = std::next(__first, n / 2);
    
    auto future = std::async(std::launch::async, [=, &__op] () {
        return par::count_if(__first, __middle, __op, chunk);
    } );
    
    std::size_t num = par::count_if(__middle, __last, __op, chunk);
    return num + future.get();
}

// front-facing chunk calculator function
template <class _ForwardIt, class _UnaryOperation>
std::size_t count_if(_ForwardIt __first, _ForwardIt __last, _UnaryOperation __op) {
    std::size_t n = static_cast<std::size_t>(std::distance(__first, __last));
    std::size_t n_cores = std::thread::hardware_concurrency();
    std::size_t chunk = std::max(n / n_cores * 32, std::size_t(1000));
    
    return par::count_if(__first, __last, __op, chunk);
}

} //namespace par

int main()
{
    int x = 3;
    std::vector<int> ivec = { 1, 2, 3, 3, 3, 3, 4 };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // normal std::count_if
    std::cout << "std::count_if: "
              << std::count_if(ivec.begin(), ivec.end(),[&x] (int v) { return v == x; } )
              << '\n';
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // par::count_if including chunk size
    std::cout << "par::count_if: "
              << par::count_if(ivec.begin(), ivec.end(), [&x] (int v) { return v == x; }, 1024)
              << '\n';
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // par::count_if with precalculated chunk
    std::cout << "par::count_if: "
              << par::count_if(ivec.begin(), ivec.end(), [&x] (int v) { return v == x; } )
              << '\n';
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// std::count_if: 4
// par::count_if: 4
// par::count_if: 4
// Program ended with exit code: 0
