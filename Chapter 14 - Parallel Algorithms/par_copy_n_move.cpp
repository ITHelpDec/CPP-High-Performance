#include <iostream>
#include <vector>
#include <future>

namespace par {

template <typename _InputIt, typename _OutputIt, typename _Predicate>
_OutputIt copy_if(_InputIt __first, _InputIt __last, _OutputIt __result, _Predicate __pred, std::size_t __chunk_sz) {
    // Part 1 - filter out the chunks according to the predicate
    const std::size_t n = static_cast<std::size_t>(std::distance(__first, __last));
    
    std::vector<std::future<std::pair<_OutputIt, _OutputIt>>> futures;
    futures.reserve(n / __chunk_sz);
    
    for (std::size_t i = 0; i < n; i += __chunk_sz) {
        const std::size_t stop_index = std::min(i + __chunk_sz, n);
        
        auto future = std::async( [=, &__pred] () {
            _OutputIt __result_first = __result + i;
            _OutputIt __result_last = std::copy_if(__first + i, __first + stop_index, __result_first, __pred);
            return std::make_pair(__result_first, __result_last);
        } );
        
        futures.emplace_back(std::move(future));
    }
    
    // Part 2 - move the successfully-filtered elements to the front of the container
    auto new_end = futures.front().get().second;
    
    for (auto it = std::next(futures.begin()); it != futures.end(); ++it) {
        auto chunk_range = it->get();
        new_end = std::move(chunk_range.first, chunk_range.second, new_end);
    }
    
    return new_end;
}

} // namespace part

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


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// ivec:      1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9
// ivec_copy: 1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9
// ivec:      1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9
// ivec_copy: 1 3 5 7 9 1 3 5 7 9
// Program ended with exit code
