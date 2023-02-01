# C++ High Performance (2nd Edition)

## Highlights from Chapter 9 - "Essential Utilities"

### Homegenous vs heterogenous (or [homogeneous vs heterogeneous](https://english.stackexchange.com/questions/288542/homogenous-versus-homogeneous))
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
### ...work in progress
