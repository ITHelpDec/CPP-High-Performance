#include <iostream>
#include <future>
#include <vector>

namespace par {

template <typename _InputIt, typename _OutputIt, typename _Predicate>
void copy_if(_InputIt __first, _InputIt __last, _OutputIt __result, std::atomic_size_t &__result_index, _Predicate __pred, std::size_t __chunk_sz) {
    const std::size_t n = static_cast<std::size_t>(std::distance(__first, __last));
    
    if (n <= __chunk_sz) {
        std::for_each(__first, __last, [&] (const auto &v) {
            if (__pred(v)) {
                auto __write_index = __result_index.fetch_add(1);
                *std::next(__result, __write_index) = v;
            }
        } );
        return;
    }
    
    _InputIt __middle = std::next(__first, n / 2);
    
    auto future = std::async( [__first, __middle, __result, &__result_index, &__pred, __chunk_sz] () {
        par::copy_if(__first, __middle, __result, __result_index, __pred, __chunk_sz);
    } );
    
    par::copy_if(__middle, __last, __result, __result_index, __pred, __chunk_sz);
    future.wait();
}

template <typename _InputIt, typename _OutputIt, typename _Predicate>
_OutputIt copy_if(_InputIt __first, _InputIt __last, _OutputIt __result, _Predicate __pred, std::size_t __chunk_sz) {
    std::atomic_size_t __result_index = 0;
    par::copy_if(__first, __last, __result, __result_index, __pred, __chunk_sz);
    return std::next(__result, __result_index);
}

} // namespace par

void printVec(const std::vector<int> &ivec) {
    for (const int i : ivec) {
        std::cout << i << ' ';
    } std::cout << '\n';
}

int main()
{
    std::vector<int> ivec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, ivec_copy(ivec);
    std::cout << "ivec:      "; printVec(ivec);
    std::cout << "ivec_copy: "; printVec(ivec_copy);
    
    auto from_here_till = par::copy_if(ivec.begin(), ivec.end(), ivec_copy.begin(), [] (auto &&v) { return v % 2; }, std::thread::hardware_concurrency());
        
    ivec_copy.erase(from_here_till, ivec_copy.end());
    
    std::cout << "ivec:      "; printVec(ivec);
    std::cout << "ivec_copy: "; printVec(ivec_copy);
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// ivec:      1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9 
// ivec_copy: 1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9 
// ivec:      1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9 
// ivec_copy: 1 3 5 7 9 1 3 5 7 9 
// Program ended with exit code: 0
