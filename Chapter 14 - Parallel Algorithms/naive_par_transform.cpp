#include <string>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
#include <future>

template <class _InputIt, class _OutputIt, class _UnaryOperation>
_OutputIt naive_par_transform(_InputIt __first, _InputIt __last, _OutputIt __result, _UnaryOperation __op) {
    std::size_t n = static_cast<std::size_t>(std::distance(__first, __last));
    std::size_t n_cores = std::thread::hardware_concurrency();
    std::size_t n_tasks = std::max(n_cores, std::size_t(1));
    std::size_t chunk_sz = (n + n_tasks - 1) / n_tasks;
    
    std::vector<std::future<void>> futures;
    
    for (auto i = 0ul; i != n_tasks; ++i) {
        auto start = chunk_sz * i;
        if (start < n) {
            auto stop = std::min(chunk_sz * (i + 1), n);
            auto fut = std::async(std::launch::async, [__first, __result, start, stop, __op] () {
                std::transform(__first + start, __first + stop, __result + start, __op);
            });
            futures.emplace_back(std::move(fut));
        }
    }
    
    for (auto &&fut : futures) { fut. wait(); }
    
    return __result;
}

int main()
{
    std::string s = "hGgjkBkGGYufYjvHIUgIUvIYgtNJvJgCgjCjgCjVJHBJKFjhVdsffRtrg";
    std::string s_t(s), s_par(s);
    
    std::cout << "s:     " << s << "\ns_t:   " << s_t << "\ns_par: " << s_par << "\n\n";
    
    std::transform(s.begin(), s.end(), s_t.begin(), [] (auto c) { return tolower(c); } );
    std::cout << "s:     " << s << "\ns_t:   " << s_t << "\ns_par: " << s_par << "\n\n";
    
    naive_par_transform(s.begin(), s.end(), s_par.begin(), [] (auto c) { return tolower(c); } );
    std::cout << "s:     " << s << "\ns_t:   " << s_t << "\ns_par: " << s_par << "\n\n";
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// s:     hGgjkBkGGYufYjvHIUgIUvIYgtNJvJgCgjCjgCjVJHBJKFjhVdsffRtrg
// s_t:   hGgjkBkGGYufYjvHIUgIUvIYgtNJvJgCgjCjgCjVJHBJKFjhVdsffRtrg
// s_par: hGgjkBkGGYufYjvHIUgIUvIYgtNJvJgCgjCjgCjVJHBJKFjhVdsffRtrg

// s:     hGgjkBkGGYufYjvHIUgIUvIYgtNJvJgCgjCjgCjVJHBJKFjhVdsffRtrg
// s_t:   hggjkbkggyufyjvhiugiuviygtnjvjgcgjcjgcjvjhbjkfjhvdsffrtrg
// s_par: hGgjkBkGGYufYjvHIUgIUvIYgtNJvJgCgjCjgCjVJHBJKFjhVdsffRtrg

// s:     hGgjkBkGGYufYjvHIUgIUvIYgtNJvJgCgjCjgCjVJHBJKFjhVdsffRtrg
// s_t:   hggjkbkggyufyjvhiugiuviygtnjvjgcgjcjgcjvjhbjkfjhvdsffrtrg
// s_par: hggjkbkggyufyjvhiugiuviygtnjvjgcgjcjgcjvjhbjkfjhvdsffrtrg

// Program ended with exit code: 0
