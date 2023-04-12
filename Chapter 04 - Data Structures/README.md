# C++ High Performance (2nd Edition)

## Highlights from Chapter 4 - "Data Structures"

### Latency numbers every programmer should know

| Reference             | Latency  |
| :--- | --- |
| L1 cache reference    |   0.5 ns |
| L2 cache reference    |   7   ns |
| Main memory reference | 100   ns |

* Temporal locality: Accessing data that has recently been used (like dynamic programming)
* Spatial locality: Accessing data near some other data you are using
* Cache thrashing: Constantly wiping out the cache lines in the likes of inner loops
#
### Moral of the story
Organise your memory contiguously

[loop_interchange.cpp](loop_interchange.cpp)
#
### Additions to std::vector from C++20
```cpp
std::vector<int> ivec = { -1, 5, 2, -3, 4, -5, 5 };
std::erase(v, 5);
std::erase_if(v, [] (const int &x) { return x < 0; } );
```
[vector.cpp](vector.cpp)
#
### Smarter use of std::array
[array.cpp](array.cpp)
#
### `std::basic_string`
The biggest change recently has been from C++17.

> _"Historically, std::basic_ string was not guaranteed to be laid out contiguously in memory. This changed with C++17, which makes it possible to pass the string to APIs that require an array of characters"_ – pg. 109

Really interesting use of unformatted I/O with std::basic_string

[basic_string.cpp](io/basic_string.cpp) | [file.txt](io/file.txt)

> _"Most implementations of std::basic_string utilize something called small object optimization, which means that they do not allocate any dynamic memory if the size of the string is small"_ – pg. 109
#
### Hash and equals
Rather than rely on the red-black tree implementation found in std::set, we can specify our own hash function (I've done this previously in C++ Primer).

[bad_hash.cpp](bad_has.cpp) | [combine_hash.cpp](combine_hash.cpp) | [template_specialisation.cpp](template_specialisation.cpp)

Also take a look at the code below to see use of the new bit-rotation functions that have come as part of C++20 – they mimic the Assembly instructions `ror` and `rol` instructions so that we don't have to create our own e.g.
```cpp
inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}
```
```asm
func(unsigned long, unsigned int):
    mov     ecx, esi
    rol     rdi, cl
    mov     rax, rdi
    ret
```

Thanks go to Pelle Evensen for his work on this hash function variation.

[rrxmrrxmsx_0.cpp](rrxmrrxmsx_0.cpp)
#
### Priority Queues
Interesting use of std::priority_queue for collecting the `k` largest elements, although I've modified the algotihm slightly to avoid the use of `std::reverse` (seems unnecessarily expensive).

I'd also be interested in modifying the implementation to use a `std::set` or `std::multiset`, or even a `min_heap` to display the elements in the correct order, and hopefully avoid the `while` loop altogether.

[document.cpp](document.cpp)
#
### `std::string_view`
This is a really interesting new feature as part of C++17 - compared to `std::string`, the simple benchmarks I ran on constructing string literals were really positive, although on my machine I find std::chrono::high_resolution_clock is always quite slow on the first test, so I've alternated the constructions with different words to see if there's a noticeable difference.

`std::string_view` seems to be quite fast, so I might start using it more often.

[string_view.cpp](string_view.cpp)
#
### `std::span`
Seems a bit of pointless...but it's just a mutable `std::string_view` in an attempt to eliminate array decay.
```cpp
void func(std::span<float> buffer) {
    for (auto &&b : buffer) { std::cout << b << " "; }
} // why the auto? what is it?
// it's not a float&&, although const float& works in its place
```
#
### Performance overheads
Turns out...

`if (mset.lower_bound(word) != mset.end())`

...is faster than...

`if (mset.find(word) != mset.end())`

..., and both are faster than 

`if (mset.contains(words)`

...so I'll be using `lower_bound` more often.

[find_benchmarks.cpp](find_benchmarks.cpp)
#
### Size matters
...for spatial locality, clearly.
> _"When iterating over elements stored contiguously in memory, we will increase the probability that the data we need is already cached if we manage to keep our objects small, thanks to spatial locality. Obviously, this will have a great impact on performance"_ – pg. 123

Iterating over an object of 8 bytes compared to an object of 260 bytes is twice as fast on my machine, based off the benchmark listed below.

[sizeof_benchmark.cpp](sizeof_benchmark.cpp)
#
### Parallel Arrays
Basically, the concept of de-encapsulating information from a class into an array to allow for faster iteration.

Reminds me of Chapter 1 with how Java handles memory allocation compared to C++ – `std::shared_ptr`might be an interesting way to allow for parallel arrays without de-encapsulation.

Source code available on author's GitHub.
#
### Summary
Really interesting chapter – I'm looking forward to learning more about std::string_view and temporal / spatial locality.
#
### If you've found anything from this repo useful, please consider contributing towards the only thing that makes it all possible – my unhealthy relationship with 90+ SCA score coffee beans.

<a href="https://www.buymeacoffee.com/ITHelpDec"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=ITHelpDec&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff" /></a>
