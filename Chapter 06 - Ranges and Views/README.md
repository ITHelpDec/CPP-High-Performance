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
