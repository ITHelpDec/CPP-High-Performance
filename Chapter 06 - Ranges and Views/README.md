# C++ High Performance (2nd Edition)

## Highlights from Chapter 6 - "Ranges and Views"

### `std::views`
We can use `std::views` to refactor code into something more legible, although the time space complexity of the author's algorithm is a bit crud.

[get_max_score.cpp](get_max_score.cpp) | [get_max_score_views.cpp](get_max_score_views.cpp)
#
### `std::views::join`
Interesting feature to flatten out a nested array

[flatten.cpp](flatten.cpp)
#
### Improved legibility with namespace
Turn 3 lines into 2.

Still disappointing that this [only compiles on GCC as opposed to Clang](https://godbolt.org/z/xG3Pz4GP6) as of January 2023 – sort of defeats the purpose of portability (the standard's been out for 3 years, like...).

[get_max_score_refactor.cpp](get_max_score_refactor.cpp)
#
### Ownership
> _"Containers own their elements, so we can, therefore, call them owning ranges"_ – pg. 180

> _"A view is also a range, that is, it provides begin() and end() functions. However, unlike containers, a view does not own the elements in the range that the view spans over."_ – pg.180

Ownership seems to be a big thing with pointers and smart pointers, so I get the feeling this could be quite important as the chapter progresses.

`std::views` also allow for $O(1)$ run-time, compared to $O(n)$ with `std::ranges`
#
### Materialising a `std::view`
A few different methods of transposing `T<e>` to `U<f>` e.g. `std::list<int>` -> `std::vector<std::string>`

[materialise.cpp](materialise.cpp) | [generic.cpp](generic.cpp)
#
### `std::views` are lazy-evaluated
Views are lazy, materialisations are eager.
> _"Remember that once the view has been copied back to a container, there is no longer any dependency between the original and the transformed container. This also means that the materialization is an eager operation, whereas all view operations are lazy."_ – pg. 183
#
### Sorting
It's not possible to sort `std::views` because they are lazy-evaluated e.g.
```cpp
std::vector<int> ivec = { 1, 2, 3, 4, 5 };
auto odd_numbers = ivec
    | std::views::filter( [] (auto i) { return i % 2; } );
    
// does not compile
// std::ranges::sort(odd_numbers);

// materialise, then sort
std::vector<int> odd_vec = to_vector(odd_numbers);
std::ranges::sort(odd_vec);
```
This whole chapter and its syntax reminds me a lot of `grep` in linux e.g.
```bash
cat README.md | grep "woof"
```
...if you want to print out the contents of a file, but filter the output to only show what you're interested in.

...CORRECTION: it's possible to sort if we use `std::views::take`.

[lazy.cpp](lazy.cpp) | [take.cpp](take.cpp)


#
### Helpful functions
`std::views::split` and `std::views::join`
```cpp
std::string csv = "10,11,12";

auto digits = csv
    | std::views::split(',')        // [ [1, 0], [1, 1], [1, 2] ]
    | std::views::join;             // [ 1, 0, 1, 1, 1, 2 ]

for (const auto &i : csv) {
    std::cout << i << " ";          // "1 0 1 1 1 2"
} std::cout << std::endl;
```
`std::views::drop`
```cpp
std::vector ivec = { 1, 2, 3, 4, 5, 4, 3, 2, 1 };

auto v = ivec
  | std::views::drop_while( [] (auto i) { return i < 5; } )     // drop all elements until condition is not met
  | std::views::take(3);                                        // take the first 'n' elements from that point

for (const auto &i : v) {
    std::cout << i << " ";                                      // 5, 4, 3
} std::cout << std::endl;
```
#
### Summary
Shame this isn't widely supported unless using gcc, but it feels like Python met Linux and had a syntactical baby in order to create it.

I can't see myself wanting to use this any time soon, but if I do then I might give range-v3 a go.

I did, however, find out that Xcode supports...
```cpp
#include <__ranges/all>
```
...as well as a few other inidividual header files, so that was one step closer to getting things to run successfully on Xcode.

All-in-all, biggest takeaway was that `view` construction is $O(1)$ time, lazily-evaluated and non-owning.
#
### If you've found anything from this repo useful, please consider contributing towards the only thing that makes it all possible – my unhealthy relationship with 90+ SCA score coffee beans.

<a href="https://www.buymeacoffee.com/ITHelpDec"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=ITHelpDec&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff" /></a>
