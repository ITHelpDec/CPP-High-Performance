#include <tuple>
#include <vector>
#include <iostream>

template <typename T, typename V>
bool in_variant(const V &v, T t) {
    return std::holds_alternative<T>(v) && std::get<T>(v) == t;
}

template <typename T, typename V>
bool in_variant_vec(const std::vector<V> &vvec, T t) {
    return std::any_of(vvec.begin(), vvec.end(), [&t] (const V &v)
                       { return std::holds_alternative<T>(v) && std::get<T>(v) == t; } );
}

int main()
{
    std::cout << std::boolalpha;
    
    std::variant<int, const char*> v = "woof";
    std::cout << "woof in v: " << in_variant(v, "woof") << '\n';
    std::cout << "bark in v: " << in_variant(v, "bark") << '\n';
    
    std::vector<std::variant<int, const char*>> vvec = { "man", "bear", "pig", 69 };
    std::cout << "woof in vvec: " << in_variant_vec(vvec, "woof") << '\n';
    std::cout << "man  in vvec: " << in_variant_vec(vvec, "man") << '\n';
    std::cout << "69   in vvec: " << in_variant_vec(vvec, 69) << '\n';
    std::cout << "68   in vvec: " << in_variant_vec(vvec, 68) << '\n';
    
    std::cout << std::noboolalpha;
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// woof in v: true
// bark in v: false
// woof in vvec: false
// man  in vvec: true
// 69   in vvec: true
// 68   in vvec: false
// Program ended with exit code: 0
