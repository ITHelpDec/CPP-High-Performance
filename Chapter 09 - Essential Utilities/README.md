# C++ High Performance (2nd Edition)

## Highlights from Chapter 9 - "Essential Utilities"

### Homogenous vs heterogenous (or [homogeneous vs heterogeneous](https://english.stackexchange.com/questions/288542/homogenous-versus-homogeneous))
Homogeneous containers
* `std::vector<int>`
* `std::list<Boat>`

Heterogeneous
* `std::optional`
* `std::pair`, `std::tuple`, `std::tie()`
* `std::any`, `std::variant`

#
### `std::optional'
> _"In a nutshell, it is a small wrapper for any type where the wrapped type can be either initialized or uninitialized."_ – pg. pg. 276

> _"To put it in C++ lingo, std::optional is a stack-allocated container with a max size of one."_ – pg. 276

> _"It's also possible to
access the value using the value() member function, which instead will throw an std::bad_optional_access exception if the optional contains no value."_ – pg. 277

<details>
<summary>std::optional in action</summary>

```cpp
#include <optional>

struct Point { /*...*/ };
struct Line { /*...*/ };

bool lines_are_parallel(const Line &a, const Line &b) { return false; }

Point compute_intersection(const Line &a, const Line &b) { return Point(); }

std::optional<Point> get_intersection(const Line &a, const Line &b)
{
    if (lines_are_parallel(a, b)) {
        return std::optional(compute_intersection(a, b));
    } else {
        return { }; // or return std::nullopt;
    }
}

void set_magic_point(Point p) { /*...*/ };

int main()
{
    std::optional<Point> intersection = get_intersection(Line(), Line());
    
    if (intersection.has_value()) { set_magic_point(*intersection); }
    
    return 0;
}
```
</details>

> _"The object held by a std::optional is always stack allocated, and the memory overhead for wrapping a type into a std::optional is the size of a bool (usually one byte), plus possible padding."_ – pg. 277

<details>
<summary>Another example</summary>

```cpp
#include <cassert>
#include <optional>

struct Hat { };

class Head {
public:
    Head() { assert(!hat_); }
    
    void set_hat(const Hat &h) { hat_ = h; }
    
    bool has_hat() const { return hat_.has_value(); }
    
    // auto get_hat() const {
    // has to return Hat if assertion is to pass
    Hat get_hat() const {
        assert(hat_.has_value());
        return *hat_;
    }
    
    void remove_hat() { hat_ = std::nullopt; }
    
private:
    std::optional<Hat> hat_;
};
```
</details>

> _"Without std::optional, representing an optional member variable would rely on, for example, a pointer or an extra bool member variable. Both have disadvantages such as allocating on the heap, or accidentally accessing an optional considered empty without a warning."_ – pg. 278

#
### Using `std::optional` with `enum`
Out with the old...
```cpp
enum class Colour { red, black, none };
Colour get_colour();
```
...and in with the new!
```cpp
enum class Colour { red, black };
std::optiona<Colour> get_colour();
```
#
### Sorting and comparisons
* Two empty `std::optional` containers are considered equal
* An empty `std::optional` container is considered less than a non-empty container
```cpp
std::vector<std::optional<int>> optivec = { { 3 }, { 2 }, { 1 }, { }, { } };
std::sort(optivec.begin(), optivec.end(); // { { }, { }, { 1 }, { 2 }, { 3 } }
```
`std::optional` is an _"efficient and safe alternative"_ to previous methods, but I would like to run or see some more benchmark comparisons and implementations before making up my mind.
#
### `std::pair` and `std::tuple`
Both of these heterogenous / hetergeneous containers can be instantiated with an arbitrary size at compile time.
#
### `std::pair`
`std::minmax()` from earlier is a great example of where to use pairs, as is `std::equal_range`
```cpp
std::pair<int, int> mm = std::minmax( { 1, 2, 3, 4, 5 } ); // [1, 5]
```
Interesting to see that using `.emplace()` in `std::map` is more efficient than using `.insert()` (benchmark [here](https://godbolt.org/z/Ys7MM9qns)).

Structured bindings are also the bee's knees when it comes to readability over p.first and p.second (love structured bindings).
```cpp
for (const auto &[key, value] : map) {
    std::cout << key << ' ' << value << '\n';
} std::cout << '\n';
```
#
### `std::tuple`
A nice addition to the book – given the comparision to structs on page 281 – might be that `std::tuple` also falls victim to the padding dilemma covered in [Chapter 7](../Chapter%2007%20-%20Memory%20Management#padding) when types are laid out without due prior consideration.

[sizeof_tuple.cpp](sizeof_tuple.cpp)

I have more detailed examples of tuples in my C++ Primer repo [here](https://github.com/ITHelpDec/CPP-Primer/search?q=std%3A%3Atuple).

In terms of calling variables, if our tuple contains only one of any kind (like some sort of non-space-sacing `union`) then we can pass a typename parameter to `std::get` to retieve it.
```cpp
std::tuple<int, bool, std::string> t = { 69, true, "nice" };
std::get<std::string>(t); // nice
```
#
### Iterating through a `std::tuple`
Now **_this_** is juicy...

There are some great examples of how to port some useful STL algorithms, such that they can be used with typles, but with `std::apply`, the author's code can be shrunk down to one line and templatised thereafter.
```cpp
std::apply([] (auto&& ...e) { ( (std::cout << e << ' '), ...); }, t);
```
[for_each_tuple.cpp](for_each_tuple.cpp) | [any_of_tuple.cpp](any_of_tuple.cpp)
#
### Structured bindings
Structured bindings are the best - assigning multiple variables went from...
```cpp
std::string name;
int n_moons;
bool rings;

std::tie(name, n_moons, rings) = std::tuple("Saturn", 82, true);
```
...to...
```cpp
const auto &[name, n_moons, rings] = std::tuple("Saturn", 82, true);
```
#
### Variadic templates
I have loads of variadic template work from Chapter 16 of my C++ Primer repo [here](https://github.com/ITHelpDec/CPP-Primer/search?q=...Args), but the typical structure is below.
```cpp
template <typename ...Args>
void (Args ...rest) {
    performActionOn(rest...);
}
```
#
### `std::variant`
Basically a `union` without the need for an `enum` - it stores its token internally in a `std::size_t`.
```cpp
sizeof(std::variant<std::string>>) == sizeof(std::string) + sizeof(std::size_t)
```
We use `std::holds_alternative<T>(std::variant<Args...>)` to sanity check for specific elements and change the token.

Will have to benchmark at some point to measure the difference between it and a union.
### Handling exceptions with `std::variant`
> _"When a new value is assigned to a std::variant object, it is placed in the same location as the currently held value of the variant."_

> _"If, for some reason, the construction or assignment of the new value fails and throws an exception, the old value may not be restored. Instead, the variant can become valueless."_

> "_You can check whether a variant object is valueless by using the member function valueless_by_ exception(). This can be demonstrated when trying to construct an object using the emplace() member function"_ – pg. 293

[Widget.cpp](Widget.cpp)

#
### `std::visit`
Using `std::visit` to access the love element of a variant
```cpp
std::apply( [] (auto &&e) { std::cout << e << '\n'; } , v);
```
[visit.cpp](visit.cpp)

#
### Heterogen(e)nous containers holding `std::variant`
Two template functions to return if a certain item is found in a variant or a vector of variants.

[in_variant_vec.cpp](in_variant_vec.cpp)

N.B. always remember to include a `return` if it's needed - template functions have terrible error messages, and it'll save you a lot of heartache.

#
### Versions of `std::get`
We've used them already, but to summarise:
```cpp
std::get<index>();
std::get<T>();
```

#
### Real-world applications
* __Sorting structs (projection)__

The author's `if ... else ...` predicates are a touch verbose; they can defintely be refactored into something more legible (as I've done in the examples below), but this is a good exercise for refactoring – you can also start to see the benefits of the `std::ranges` library coming through in this example.

[the_quest_for_terseness.cpp](the_quest_for_terseness.cpp)

* __Collating members in a tuple (reflection)__

We can collate members into a tuple for easier iteration using `std::tie` – as much of a hard time as I give `auto`, this is a situation where it really comes in handy.

[reflection.cpp](reflection.cpp)

#
### ...work in progress
