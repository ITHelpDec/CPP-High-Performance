# C++ High Performance (2nd Edition)

## Highlights from Chapter 3 - "Analysing and Measuring Performance"

### Big O Notation
A really important concept to learn in regards to data structures and algorithms.

Some example sorts from the book.

[linear_search.cpp – O(n)](linear_search.cpp) | [binary_search.cpp – O(log(n))](binary_search.cpp)
#
### Amortised time
Amortised ≠ average
> _"Amortised running time is used for analysing a sequence of operations rather than
a single one. We are still analysing the worst case, but for a sequence of operations. The amortized running time can be computed by first analysing the running time of the entire sequence and then dividing that by the length of the sequence"_ – pg. 75

> _"It will run in O(1) in almost all cases, except very few where it will perform worse."_ – pg. 76
