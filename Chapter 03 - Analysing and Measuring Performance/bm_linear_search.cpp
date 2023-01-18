#include <benchmark/benchmark.h>
// #include <vector>
#include <numeric>

std::vector<int>::const_iterator linear_search(const std::vector<int> &vals, int key) {
    for (auto it = vals.begin(); it != vals.end(); ++it) {
        if (*it == key) { return it; }
    } return vals.end();
}

// strayed away from gen_vec() function on the off chance that constant push_back() calls might cause memory reallocation
// we already know ivec will be of size "n", so might as well use the appropriate constructor
static void bm_linear_search(benchmark::State& state) {
    int n = 1024;
    std::vector<int> ivec(n);
    std::iota(ivec.begin(), ivec.end(), 0);
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(linear_search(ivec, n));
    }
}

BENCHMARK(bm_linear_search);
BENCHMARK_MAIN();

// output:
// Run on (8 X 2700 MHz CPU s)
// CPU Caches:
//   L1 Data 32 KiB
//   L1 Instruction 32 KiB
//   L2 Unified 256 KiB (x4)
//   L3 Unified 6144 KiB
// Load Average: 1.69, 1.84, 1.86
// -----------------------------------------------------------
// Benchmark                 Time             CPU   Iterations
// -----------------------------------------------------------
// bm_linear_search        361 ns          361 ns      1945664 (author's 2018 MacBook Pro – Intel Quad-Core i7 CPU)
// bm_linear_search      18641 ns        18500 ns        39049 (my 2013 MacBook Pro – 2.7 GHz Quad-Core Intel Core i7)
// bm_linear_search      18538 ns        18396 ns        34680 (using gen_vec() instead of std::iota - similar time, but fewer iterations)
