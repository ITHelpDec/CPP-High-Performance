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

Better examples of how to calculate ${F}_ {par}$ for a function would have gone down well.

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

One of the pitfalls for this implementation is that it would only scale well if all chunk sizes were the same - in real life, though, this is apparently rarely the case.

#
### Divide and Conquer
An interesting name for a sub-chapter - you would frequently see divide and conquer employed in recurise functions like merge sort, which recursively (and logarithmically in this case) _divides_ itself into base cases, _conquers_ said base cases then _combines_ the sub-tasks into one final solution.

Other examples of _Divide and Conquer_ algorithms would be the likes of:
* Substitution method
* [Recusion-tree and "Master" methods](https://www.cs.cornell.edu/courses/cs3110/2012sp/lectures/lec20-master/lec20.html)
* [Akra-Bazzi method](https://www.isa-afp.org/browser_info/current/AFP/Akra_Bazzi/document.pdf)

[divide_and_conquer.cpp](divide_and_conquer.cpp) | [divide_and_benchmark.cpp](divide_and_benchmark.cpp)

It's a shame GitHub doesn't have good barchart markdown - would have been nice to convert the results in the benchmark into something visual (here's a link to [quick-bench](https://quick-bench.com/q/V_Nf8u0ZOIIs2y_LfGjebjCMwQQ) anyway).

#
### Applying divide and conquer to `std::count_if`
A similar logarithmic pattern here with `std::count_if`, this time named `par::count_if` (thought it might be cleaner to give the parallel functions their own namepsace with a similar function name instead of a longer function name) – I'll have to construct a benchmark of how both compare in terms of performance.

The chunk-calculating helper function is a nice abstraction, allowing you to either specify the chunk size manually or allow the hardware to decide.

[divide_and_count_if.cpp](divide_and_count_if.cpp)

#
### Applying parallelism to `std::copy_if`
Below is how `std::copy_if` is written in the standard library.
```cpp
template <typename _InputIt, typename _OutputIt, typename _Predicate>
_OutputIt copy_if(_InputIt __first, _InputIt __last, _OutputIt __result, _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    } return __result;
}
```
Tweaking this algorithm to run in parallel is slightly more difficult than the last two, as this time we have we have a mutable iterator that needs to work concurrently.

[par_copy_if.cpp](par_copy_if.cpp)

We can split the algorithm into two seperate parts - one part copying, the other part moving.

[par_copy_n_move.cpp](par_copy_n_move.cpp)

This version of the `copy_if` algorithm performs a lot better in benchmarks compared to the other one for the following reason:

> _" it is because the cache mechanism of the hardware is trashed due to several threads writing to the same cache line"_ – pg. 486

Based on this statement (and the reference to Chapter 7), maybe we could find a way to align our chunks in such a way that the same cache line isn't thrahed like in the example (maybe that would be an interesting exercise at some point).

Parallel standard library algorithms

#
### ...work in progress
