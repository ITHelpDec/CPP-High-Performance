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

[lazy.cpp](lazy.cpp)
#
### ...next
