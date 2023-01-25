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
### ...next
