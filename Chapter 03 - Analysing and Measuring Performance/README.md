# C++ High Performance (2nd Edition)

## Highlights from Chapter 3 - "Analysing and Measuring Performance"

### Big O Notation
A really important concept to learn in regards to data structures and algorithms.

Some example sorts from the book.

[linear_search.cpp](linear_search.cpp) – $O(n)$

[binary_search.cpp](binary_search.cpp) – $O(log(n))$
#
### Amortised time
Amortised ≠ average
> _"Amortised running time is used for analysing a sequence of operations rather than
a single one. We are still analysing the worst case, but for a sequence of operations. The amortized running time can be computed by first analysing the running time of the entire sequence and then dividing that by the length of the sequence"_ – pg. 75

> _"It will run in O(1) in almost all cases, except very few where it will perform worse."_ – pg. 76
#
### Sampling profilers
The concept of sampling profilers working at fixed intervals is interesting. I'd be tempted, however, to create something that records timestamps and stacks of which functions are in use in an attempt to mirror the author's diagram.

[example_profiler.cpp](example_profiler.cpp)
#
### Microbenchmarking
> _"Eliminating calls to expensive functions is usually the most effective way of optimizing the overall performance of the program."_ – pg. 88
#
### Amdahl's Law
> _"When working with microbenchmarks, it's essential to keep in mind how big (or small) an impact the optimization of the isolated code will have on the complete program"_ – pg. 89

Two things are quite important to this law:
* The proportion of the isolated part's execution time in relation to the rest of the programme **(p)**
* The speedup of hte part we are optimising (the local speed-up) **(s)**

$$ Overall\text{ }speedup = {1 \over (1 - p) + {p \over s}} $$

My own personal take on Amdahl's Law is that whilst yes, it can be a fantastic measure of where to prioritise and focus your efforts in an existing codebase, it should not be seen as an excuse to write lazy code.
#
### Pitfalls of microbenchmarking
* The compiler might optimise isolated code differently compared to how it is optimised in the full programme
* **_"Thrash the cache!"_** (the limiting factor may be due to cache misses)
#
### Google Benchmark
Best thing from this chapter was learning that Google Benchmark allows you to see time complexity (it will be interesting to see if it also runs benchmarks on space complexity, and to learn more about Xcode Instruments).

To incorporate Google Benchmark functionality into an Xcode project, you must perform the following steps:
1) Install Google Benchmark using the terminal command
* `brew install google-benchmark`
2) Change the following settings within your project's `Build Settings` (select `All` + `Combined`)
* `Header Search Paths`
  * `/usr/local/Cellar/google-benchmark/<version>/include`
  * e.g. `/usr/local/Cellar/google-benchmark/1.7.1/include`
* `Library Search Paths`
  * `/usr/local/Cellar/google-benchmark/<version>/lib`
  * e.g. `/usr/local/Cellar/google-benchmark/1.7.1/lib`
* `Other Linker Flags`
  * `-lbenchmark`
  
Now you can run your project as you normally would, with the test output going straight to the console.

See the code below for example outputs.

[bm_linear_search.cpp](bm_linear_search.cpp) | [bm_linear_search_with_range.cpp](bm_linear_search_with_range.cpp) | [linear_vs_binary.cpp](linear_vs_binary.cpp)

**EDIT (27/01/2023):** for Apple Silicon, brew will install packages in `/opt/homebrew/Cellar`; `/usr/local/Cellar` is for Intel machines, i.e.

* Headers:   `/opt/homebrew/Cellar/google-benchmark/1.7.1/include`
* Libraries: `/opt/homebrew/Cellar/google-benchmark/1.7.1/lib`

#
### Summary
By far the most interesting part of this chapter has been Google Benchmark.

I feel even more examples could have been given (even a few leetcode examples), and maybe a run through of how to [plot the results](https://github.com/lakshayg/google_benchmark_plot), but this has been a good introduction to what we can achieve with microbenchmarking software.
#
### If you've found anything from this repo useful, please consider contributing towards the only thing that makes it all possible – my unhealthy relationship with 90+ SCA score coffee beans.

<a href="https://www.buymeacoffee.com/ITHelpDec"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=ITHelpDec&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff" /></a>
