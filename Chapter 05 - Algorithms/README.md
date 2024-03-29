# C++ High Performance (2nd Edition)

## Highlights from Chapter 5 - "Algorithms"

### `std::ranges`
Basically, a way of shortening `ivec.begin(), ivec.end()` to `ivec` – jut felt like another way to skin a cat (and it isn't supported on Apple Clang, so I'll leave it be for now.

Also, this is the second time I've seen `auto&&` - what is this particular `auto`, and why is it an rvalue?
```cpp
void print(auto&& r) {
  std::ranges::for_each(r, [](auto&& i) { std::cout << i << ' '; });
}
```
Could it not be...
```cpp
template <typename T> void print(const T &t) {
    for (const auto &e : t) {
        std::cout << e << " ";
    }
}
```
...?

Same goes for the below - looks like ranges and auto'd lambdas are the cool new thing in C++20.
```cpp
auto in = std::vector{1, 2, 3, 4};
auto out = std::vector<int>(in.size());
auto lambda = [](auto&& i) { return i * i; };
// auto rvalue again - there's a discussion on stack overflow about comparing `auto&& woof` and `std::forward<decltype(woof)>(woof)`
std::ranges::transform(in, out.begin(), lambda);
print(out);
```
#
### `std::clamp`
Introduced from C++17, this will be quite a useful feature for leetcode.
```cpp
const int y = std::max(std::min(some_func(), y_max), y_min);
```
...can become...
```cpp
const int y = std::clamp(some_func(), y_min, y_max);
```
`std::minmax()` was also a function I wasn't aware of:
```cpp
std::pair<int, int> mmp = std::minmax(ivec.begin(), ivec.end());
```
#
### Sentinel values
> _"A sentinel value (or simply a sentinel) is a special value that indicates the end of
a sequence"_ – pg. 143
#
### Iterators
Had seen `std::advance(it, n)` before, but `it += n` is a new one for me – might end up using this more often.
#
### Algorithms with output require allocated data
> _"If an iterator to an empty container is passed to the algorithms for output, the program will likely crash"_ – pg. 150

Use a `std::insert_inserter` or resize the container to avoid crashes.
```cpp
std::vector<int> squared, ivec = { 1, 2, 3, 4, 5 };

// C++20
std::ranges::transform(.:.vector.:., .:.inserter.:., .:.lambda.:.);

// pre C++20
std::transform(ivec.begin(), ivec.end(), std::back_inserter(squared), [] (const int &x) { return x * x; } );
```
#
### Three-way comparison operator
The C++20 'spaceship' is a helpful reduction that should help condense code for comparison operators.
```cpp
bool operator==(const Woof &woof1, const Woof &woof2) { return woof1 == woof2; }
bool operator!=(const Woof &woof1, const Woof &woof2) { return !(woof1 == woof2); }

bool operator< (const Woof &woof1, const Woof &woof2) { return woof1 < woof2; }
bool operator> (const Woof &woof1, const Woof &woof2) { return woof2 < woof1; }
bool operator<=(const Woof &woof1, const Woof &woof2) { return !(woof1 > woof2); }
bool operator>=(const Woof &woof1, const Woof &woof2) { return !(woof1 < woof2); }
```
...becomes...

```cpp
auto operator<=>(const Woof&) const = default;
```

The latter *_must_* be done within the body of the class, however, compared to how it is normally done by using declaring the operator as a `friend`, and _*must*_ be declared to have an `auto` return type due to the following error.

> _"No matching conversion for static_cast from 'std::strong_ordering' to 'bool'"_

I can imagine this would be helpful if all elements are being compared, and then you can drop back to the normal way if you want more control over what elements are being compared.
#
### Avoiding lambdas
These are clean ways to sort the elments of a vector by size and find elements based on criteria.
```cpp
std::sort(svec.begin(), svec.end(),
          [] (const std::string &lhs, const std::string &rhs) { return lhs.size() < rhs.size(); } );
=> std::ranges::sort(svec, std::less<>, &std::string::size);
```
and
```cpp
std::find(svec.begin(), svec.end(), [] (const std::string &s) { return s.size() == 3; } );
=> std::ranges::find(svec, 3, &std::string::size);
```
#
### Complexity guarantees
STL promises no greater complexity than $O(log(n))$.

Author's $O(n^2)$ solution is expensive.
```cpp
template <typename Iterator>
auto contains_duplicates(Iterator first, Iterator last) {
  for (auto it = first; it != last; ++it)
    if (std::find(std::next(it), last, *it) != last)
      return true;
  return false;
}
```
The following would get us to $O(n)$ time using the author's techniques without the need to sort.
```cpp
template <class ForwardIt>
bool contains_duplicates(ForwardIt first, ForwardIt last) {
    std::unordered_set<std::decay_t<decltype(*first)>> dup_set(first, last);
    return dup_set.size() != (last - first);
}
```
...or...
```cpp
template <typename T>
bool contains_duplicates(const T &t) {
    std::unordered_set<typename T::value_type> tset(t.begin(), t.end());
    return t.size() != tset.size();
}
```
#
### Sorting only the data you need
`std::nth_element` is $O(n)$ time, so that will be useful, although the author forgets to mention that this algorithm only retains its linear complexity promise for containers with 10 or more elements (at least on my machine), otherwise it will fully sort the entire container (making it $O(log(n))$ time).

Across thousands, if not millions, of elements, `std::nth_element` seems to perform very well, but I was unable to mimic the authors tests in Debug mode, only Release mode.

[sort_bechmarks.cpp](sort_benchmarks.cpp)
#
### Favour STL algorithms where reasonable
`std::rotate` is quite powerful, similar to SAR or ROTR in Assembly.

[sar.cpp](sar.cpp)
#
### Compare with zero
> _"In addition to the loop unrolling, a very subtle optimization is...[comparing] with zero instead of a value. On some CPUs, comparing with zero is slightly faster than any other value, as it uses another assembly instruction (on the x86 platform, it uses test instead of cmp)."_ – pg. 169
#
### Summary
Decent chapter.

Main takeaways were a few useful STL algorithms, the 'spaceship' and how to deduce container element type in a templated function.
#
### If you've found anything from this repo useful, please consider contributing towards the only thing that makes it all possible – my unhealthy relationship with 90+ SCA score coffee beans.

<a href="https://www.buymeacoffee.com/ITHelpDec"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=ITHelpDec&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff" /></a>
