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
### `consteval`
> _"A function that is declared using consteval is called an immediate function and can only produce constants"_ – pg. 248

```cpp
consteval int sum(int x, int y, int z) { return x + y + z; }
constexpr int value = sum(3, 4, 5); // fine
int value2 = sum(4, 5, 6); // not so fine
```
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
### Checking programming errors at compile time
> _"...if we have a constant expression, we can catch programming errors when compiling the program using `static_assert()`."_ – pg. 253

* `assert` - runtime asserts
* `static_assert` - compile-time asserts

With these in mind, favouring `static_assert` would allow us to avoid writing / compiling programmes that might lead to runtime errors later down the line.
#
### Unconstrained `Point2D` template
Could use multiple typenames, but `auto` here can tidy up the code up nicely - just need to be careful not to make it overly-generic in case someone decides to pass a string as an argument (EDIT: this is actually covered in the next few pages).

The typename solution doesn't scale well beyond two arguments, but provides a more understandable autocomplete within the IDE, so it's easier to know what parameters are expected.

Regardless, the class and function templates have been tweaked to allow for more flexibility in the arguments provided.

```cpp
dist(const Point2D<T, U> &p1, const Point2D<V, W> &p2)
```
[Point2D.cpp](Point2D.cpp)
#
## Concepts
A big new topic as of C++20
```cpp
template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;
```
```cpp
template <typename T>
concept Number = FloatingPoint<T> || std::is_integral_v<T>;
```
<details>
<summary>definition of concept std::range in Ranges library</summary>

```cpp
// compiles on Xcode
// maybe this will allow me to run my code from earlier chapters
#include <__ranges/concepts.h>

template <typename T>
concept range = requires(T &t) {
    std::ranges::begin(t);
    std::ranges::end(t);
};
```

</details>
<details>
<summary>std::integral</summary>

``` cpp
#include <concepts>
#include <iostream>

// from the book (not a great example)
std::integral auto mod(std::integral auto v, std::integral auto n)
{
    return v % n;
}

int main()
{
    // fine
    std::cout << mod(3, 2) << '\n';     // 1
    
    // legal, but defeats the purpose of all the constraints
    std::cout << mod('o', 2) << '\n';   // 1
    
    return 0;
}

```

</details>

#
### `Point2D` with concepts
The example linked below now mirrors what my previous template design did (slight modification from book to include mixed types).

Considering they do the same thing, I'll have to benchmark the differences between concepts and templates to see which is faster.

[Point2D_concepts.cpp](Point2D_concepts.cpp)

#
### Compile-time string optimisations
This is fascinating.
> _"In order to enable the compiler to calculate the hash sum at compile time, we rewrite hash_function() so that it takes a raw null-terminated char string as a parameter [instead] of an advanced class like std::string, which cannot be evaluated at compile time."_ – pg. 270

If this is the case and performance is the goal, then perhaps it would be better for favour `const char*` over `std::string` under certain circumstances?
```cpp
constexpr std::size_t hash_function(const char* str) {
    size_t sum = 0;
    for (auto ptr = str; *ptr != '\0'; ++ptr)
        sum += *ptr;
    return sum;
}
```
I've done a Google Benchmark of this hasher function using `const char*`, `std::string` and `std::string_view`, and `const char*` came out trumps. `std::string_view` wasn't far behind, but `std::string` was about 4-5 times slower on my machine.

Looking at the assembly instructions on [Godbolt](https://godbolt.org/z/aKssefKcs), using `constexpr` drops the instruction count _**considerably**_ from 47 lines to 17 (10 with `-O2`, although `std::string` and `std::string_view` also reduce to 10 instructions with the same optimisations).

Seriously impressive though.

[cts_benchmark.cpp](cts_benchmark.cpp)
#
###
#
### ...work in progress
