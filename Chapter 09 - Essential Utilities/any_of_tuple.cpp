#include <iostream>
#include <tuple>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// AUTHOR VERSION  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename Tuple, typename Func, size_t Index = 0>
auto tuple_any_of(const Tuple& t, const Func& f) -> bool {
  constexpr auto n = std::tuple_size_v<Tuple>;
  if constexpr(Index < n) {
    bool success = std::invoke(f, std::get<Index>(t));
    if (success) {
      return true;
    }
    return tuple_any_of<Tuple, Func, Index+1>(t, f);
  } else {
    return false;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// SQUASHED VERSION  - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class T, typename F, std::size_t N = 0>
bool any_of_tuple(const T &t, const F &f) {
    constexpr std::size_t sz = std::tuple_size_v<T>;
    if constexpr (N < sz) {
        return std::invoke(f, std::get<N>(t)) ? true : any_of_tuple<T, F, N + 1>(t, f);
    } else {
        return false;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// APPLY VERSION - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class T, typename U>
bool any_of_tuple_short(const T &t, U &&u) {
    return std::apply([u] (auto&& ...rest) { return ( (rest == u) || ... ); } , t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main()
{
    std::tuple<int, float, double> t = { 42, 43.0f, 44.0 };
    
    auto pred = [] (const auto v) { return v == 42; };
    
    std::cout << std::boolalpha << any_of_tuple(t, pred) << '\n';
    
    // will not work with our previous example with non-integral types
    // std::tuple<int, bool, const char*> t = { 1, true, "highlander" };
    
    // the shorthand version was a little trickier to write with parameter packs...
    // ..., but is appears to resemble ranged-for loops much more closely 
    std::cout << any_of_tuple_short(t, 44) << '\n' << std::noboolalpha;
    
    return 0;
}
