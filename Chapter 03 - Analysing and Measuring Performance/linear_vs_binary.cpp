#include <benchmark/benchmark.h>
#include <numeric>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ALGORITHMS  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

std::vector<int>::const_iterator linear_search(const std::vector<int> &vals, int key) {
    for (auto it = vals.begin(); it != vals.end(); ++it) {
        if (*it == key) { return it; }
    } return vals.end();
}

template<class ForwardIt, class T>
ForwardIt stl_binary_search(ForwardIt first, ForwardIt last, const T& value) {
    first = std::lower_bound(first, last, value);
    return (!(first == last) and !(value < *first)) ? first : last;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BENCHMARKS  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void bm_linear_search(benchmark::State& state) {
    std::int64_t n = state.range(0);
    std::vector<int> ivec(n);
    std::iota(ivec.begin(), ivec.end(), 0);
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(linear_search(ivec, n));
    } state.SetComplexityN(n);
}

static void bm_binary_search(benchmark::State& state) {
    std::int64_t n = state.range(0);
    std::vector<int> ivec(n);
    std::iota(ivec.begin(), ivec.end(), 0);
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(binary_search(ivec.begin(), ivec.end(), n));
    } state.SetComplexityN(n);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RESULTS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

BENCHMARK(bm_linear_search)->RangeMultiplier(2)->Range(64, 4096)->Complexity();
BENCHMARK(bm_binary_search)->RangeMultiplier(2)->Range(64, 4096)->Complexity();

BENCHMARK_MAIN();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Run on (8 X 2700 MHz CPU s)
// CPU Caches:
//   L1 Data 32 KiB
//   L1 Instruction 32 KiB
//   L2 Unified 256 KiB (x4)
//   L3 Unified 6144 KiB
// Load Average: 1.61, 1.60, 1.83
// ----------------------------------------------------------------
// Benchmark                      Time             CPU   Iterations
// ----------------------------------------------------------------
// bm_linear_search/64         1226 ns         1219 ns       557325
// bm_linear_search/128        2416 ns         2401 ns       313560
// bm_linear_search/256        4600 ns         4572 ns       145912
// bm_linear_search/512        9639 ns         9594 ns        78521
// bm_linear_search/1024      18512 ns        18421 ns        38488
// bm_linear_search/2048      35929 ns        35734 ns        19794
// bm_linear_search/4096      72264 ns        72059 ns         9765
// bm_linear_search_BigO      17.66 N         17.60 N    
// bm_linear_search_RMS           1 %             1 %    
// bm_binary_search/64          179 ns          178 ns      4038353
// bm_binary_search/128         195 ns          194 ns      3583495
// bm_binary_search/256         211 ns          211 ns      3460584
// bm_binary_search/512         238 ns          237 ns      3141733
// bm_binary_search/1024        247 ns          246 ns      3029293
// bm_binary_search/2048        264 ns          263 ns      2734621
// bm_binary_search/4096        280 ns          280 ns      2599872
// bm_binary_search_BigO      25.21 lgN       25.15 lgN  
// bm_binary_search_RMS           7 %             7 %    
