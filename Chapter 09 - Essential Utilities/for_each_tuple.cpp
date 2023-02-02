#include <tuple>
#include <iostream>
#include <functional>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// AUTHOR'S VERSION  - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <size_t Index, typename Tuple, typename Func>
void tuple_at(const Tuple& t, Func f) {
    const auto& v = std::get<Index>(t);
    std::invoke(f, v);
}

template <typename T, typename F, size_t N = 0>
void tuple_for_each(const T &t, const F &f) {
    constexpr std::size_t sz = std::tuple_size_v<T>;
    if constexpr(N < sz) {
        tuple_at<N>(t, f);
        tuple_for_each<T, F, N + 1>(t, f);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// SQUASHED VERSION  - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class T, typename F, std::size_t N = 0>
void tuple_for_each_condensed(const T &t, const F &f) {
    constexpr std::size_t sz = std::tuple_size_v<T>;
    if constexpr (N < sz) {
        std::invoke(f, std::get<N>(t));
        tuple_for_each_condensed<T, F, N + 1>(t, f);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MORE CONCISE VERSION  - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class T>
void tuple_apply(const T &t) {
    std::apply([] (auto&& ...e) { ( (std::cout << e << ' '), ...); }, t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main()
{
    std::tuple<int, bool, const char*> t = { 69, true, "nice" };
    
    auto f = [] (const auto &e) { std::cout << e << ' '; } ;
    
    std::cout << "tuple_for_each:           "; tuple_for_each(t, f);
    std::cout << '\n';
    
    std::cout << "tuple_for_each_condensed: "; tuple_for_each_condensed(t, f);
    std::cout << '\n';
    
    std::cout << "tuple_apply:              "; tuple_apply(t);
    std::cout << '\n';
    
    return 0;
}
