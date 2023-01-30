# C++ High Performance (2nd Edition)

## Highlights from Chapter 8 - "Compile-Time Programming"

### Template metaprogramming
Template metaprogramming allows us to write code that transforms itself into regular C++ code.

![](metaprogramming.jpg)
#
### Templates
See [Chapter 16](https://github.com/ITHelpDec/CPP-Primer/tree/main/Chapter%2016%20-%20Templates%20and%20Generic%20Programming) from my C++ Primer repo for more in-depth example of templates and template specialisations.
#
### Receiving the type of a variable with `decltype`
As of C++20, we can use `std::cvref_remove` from `<type_traits>` to help with abbreviated function templates - previously we would have used `std::decay` (see next example).
<details>
<summary>Example of decltype</summary>

```cpp
#include <iostream>

// must be N then T; T then N will not compile
template <std::size_t N, typename T>
T const_pow_n(const T &t) {
    T result = 1;
    
    for (int i = 0; i != N; ++i) { result *= t; }
    
    return result;
}

auto pow_n(const auto &v, int n) {
    // decltype(v) product = 1;
    // will not compile, because it is a const& - we can use std::remove_cvref
    // Cannot assign to variable 'product' with const-qualified type 'decltype(v)' (aka 'const float &')
    typename std::remove_cvref<decltype(v)>::type product = 1;
    
    for (int i = 0; i != n; ++i) { product *= v; }
    return product;
}

// do it as a lambda instead with `typename T` - remember to change `auto` to `T`
auto pow_n_2 = [] <typename T> (const T &v, int n) {
    T product = 1;
    for (int i = 0; i != n; ++i) { product *= v; }
    return product;
};

int main()
{
    std::cout << const_pow_n<3>(3.0f) << '\n';
    
    std::cout << pow_n(3.0f, 3) << '\n';
    
    std::cout << pow_n_2(3.0f, 3) << '\n';
    
    return 0;
}
```
</details>

#
### Type traits
We can use a few techniques to compare values and types, using either the newer C++17 `_v` / `_t` style or the old `::value` / `::type` style.
<details>
<summary>Use of std::decay and type_traits</summary>

```cpp
#include <type_traits>
#include <iostream>

int main()
{
    int v = 1;
    
    // introduced as of C++20
    std::remove_cvref<decltype(v)>::type cvref_int = 2;
    
    // before this we would have used std::decay
    std::decay<decltype(v)>::type decay_int_old = 3;
    std::decay_t<decltype(v)> decay_int = 4;
    
    std::cout << "v:            " << v             << '\n';
    std::cout << "remove_cvref: " << cvref_int     << '\n';
    std::cout << "decay:        " << decay_int_old << '\n';
    std::cout << "decay_t:      " << decay_int     << '\n';
    
    return 0;
}
```
</details>
<details>
<summary>Another example of using type_traits</summary>

```cpp
#include <type_traits>
#include <iostream>

template <typename T>
int sign_func(T t) {
    if (std::is_unsigned_v<T>) { return 1; }
    return t < 0 ? -1 : 1;
}

template <typename T>
int sign_func_old(T t) {
    if (std::is_unsigned<T>::value) { return 1; }
    return t < 0 ? -1 : 1;
}

int main()
{
    std::cout << " 1: " << sign_func(1)  << ' ' << sign_func_old(1)  << '\n';
    std::cout << "-1: " << sign_func(-1) << ' ' << sign_func_old(-1) << '\n';
    std::cout << " 0: " << sign_func(0)  << ' ' << sign_func_old(0)  << '\n';
    std::cout << "-0: " << sign_func(-0) << ' ' << sign_func_old(-0) << '\n';
    
    return 0;
}
```
</details>

#
### Programming with `constexpr`
> _"An expression prefixed with the `constexpr` keyword tells the compiler that the expression should be evaluated at compile time"_ – pg. 247

```cpp
constexpr int nice = 69; // nice
```
#
### `constexpr` vs polymoprhism
It should be no surprise that things calculated at compile-time ought to run faster than those calculated at run-time.

The cmake files from the website did not work for me, so I created some rough Google benchmarks, with results showing `constexpr` being 3x faster than polymorphism (classes and benchmark comparisons are atttached below).

[AnimalPolymorphic.cpp](AnimalPolymorphic.cpp) | [AnimalConstexpr.cpp](AnimalConstexpr.cpp) | [bm_animal.cpp](bm_animal.cpp)

#
### `if constexpr` in action
I tweaked the original function from the book to make it more flexible to inputs of multiple types (the book's example forces you to use examples of matching pairs i.e. int and int, or double and double).
    
And brace yourselves - I _have_ used a trailing return type...for pure utilitarianism though, not for `auto` member-function alignment fetishisation (sorry to disappoint).
    
[generic_mod.cpp](generic_mod.cpp)

#
### ...work in progress
