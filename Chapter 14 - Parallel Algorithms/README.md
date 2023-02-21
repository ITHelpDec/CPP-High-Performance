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
Here is the original implementation from Apple Clang:
```cpp
template <class _InputIt, class _OutputIt, class _UnaryOperation>
_OutputIt transform(_InputIt __first, _InputIt __last, _OutputIt __result, _UnaryOperation __op) {
    for (; __first != __last; ++__first, (void) ++__result)
        *__result = __op(*__first);
    return __result;
}
```

[naive_par_transform.cpp](naive_par_transform.cpp)

> _"As you will see, there are no parallel algorithms presented in this chapter that can operate on input and output iterators."_ – pg. 470

#
### ...work in progress
