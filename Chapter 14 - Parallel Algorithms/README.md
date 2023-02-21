# C++ High Performance (2nd Edition)

## Highlights from Chapter 14 - "Parallel Algorithms"

### Evaluating parallel algorithms
To calculate speedup, we can use the following formula:

$$ Speedup = { {T}_ {1} \over {T}_ {n} } $$

To calculate the efficiency of a parallel algorithm, we can use a variation of the formula above:

$$ Efficiency = { {T}_ {1} \over {T}_ {n} * n } $$

To calculate the __*maximum*__ speedup of a parallel algorithm, we can use a modified version of _"Amdahl's Law"_

$$ Maximum\text{ }speedup = { 1 \over { {F}_ {par} \over n } + (1 - {F}_ {par} ) } $$

...where ${F}_ {par}$ is the fraction of the programme that can be executed in parallel.

#
### Implementing parallel `std::transform()`

[transform.cpp](transform.cpp)

#
### ...work in progress
