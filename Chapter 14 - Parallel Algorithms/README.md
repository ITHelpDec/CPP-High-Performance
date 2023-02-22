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

Better examples of how to calculate ${F}_ {par}$ for a function would be appreciated, instead of a paper with 50% on it.

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
### More Google Benchmark (finally!)
Some interesting new techniques like `MeasureProcessCPUTime()` and `UseRealTime()` with a custom arguments functions to help reduce repitition in the code - like it!
```cpp
void CustomArguments(benchmark::internal::Benchmark* b) {
    b->Arg(50)->Arg(10'000)->Arg(1'000'000) // Input size
    ->MeasureProcessCPUTime()               // Measure all threads
    ->UseRealTime()                         // Clock on the wall
    ->Unit(benchmark::kMillisecond);        // Use ms
}
```
Slightly off-topic, but I may also eat my own words from previous chapters, as Apple have just released Xcode 14.3 Beta 1 with loads of new C++ support, including `std::ranges` and `std::views` and elements from C++23.

In terms of the benchmark itself, just like the author I experienced similar if not slightly worse results using a parallel algorithm with 50 elements compared to the sequential run. The run at 10,000 elements, however, was nearly 8x faster, and at 1,000,000 elements was over 9x faster, so certainly worth it if you're dealing with a lot of elements.

1) $speedup = { 0.083 \over 0.088 } = 0.94,\text{ }efficiency = { 0.083 \over 0.088 * 12 } = 0.0785$ (50 elements)
2) $speedup = { 16.4 \over 2.15 }\text{ }= 7.63,\text{ }efficiency = { 16.3 \over 2.15 * 12 }\text{ }\text{ }= 0.636$ (10,000 elements)
3) $speedup = { 1651 \over 170 }\text{ }= 9.71,\text{ }efficiency = { 1651 \over 170 * 12 }\text{ }\text{ }= 0.809$ (1,000,000 elements)

[parallel_benchmark.cpp](parallel_benchmark.cpp)

#
### ...work in progress
