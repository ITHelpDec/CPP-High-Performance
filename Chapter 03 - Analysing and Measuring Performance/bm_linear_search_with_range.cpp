#include <benchmark/benchmark.h>
// #include <vector>
// #include <cinttypes>
#include <numeric>

std::vector<int>::const_iterator linear_search(const std::vector<int> &vals, int key) {
    for (auto it = vals.begin(); it != vals.end(); ++it) {
        if (*it == key) { return it; }
    } return vals.end();
}

static void bm_linear_search(benchmark::State& state) {
    // Implicit conversion loses integer precision: 'int64_t' (aka 'long long') to 'int'
    std::int64_t n = state.range(0);
    std::vector<int> ivec(n);
    std::iota(ivec.begin(), ivec.end(), 0);
    
    for (auto _ : state) {
        // Implicit conversion loses integer precision: 'int64_t' (aka 'long long') to 'int'
        benchmark::DoNotOptimize(linear_search(ivec, n));
    }
}

// seems like a bit of a backwards way to specify range
// basically the additional function calls start at '64' test cases, ...
// ...and increases that amount two-fold until we reach '256' (i.e. 64 -> 128 -> 256)
BENCHMARK(bm_linear_search)->RangeMultiplier(2)->Range(64, 256);

// this feels like it would make more sense syntactically, but does not work
// BENCHMARK(bm_linear_search)->Range(64, 256)->RangeMultiplier(2);

// book missing main
BENCHMARK_MAIN();

// output:
// Run on (8 X 2700 MHz CPU s)
// CPU Caches:
//   L1 Data 32 KiB
//   L1 Instruction 32 KiB
//   L2 Unified 256 KiB (x4)
//   L3 Unified 6144 KiB
// Load Average: 3.96, 2.60, 2.36
// ---------------------------------------------------------------
// Benchmark (me)                Time             CPU   Iterations
// ---------------------------------------------------------------
// bm_linear_search/64        1201 ns         1196 ns       550353
// bm_linear_search/128       2298 ns         2285 ns       293010
// bm_linear_search/256       4856 ns         4817 ns       153332

// ---------------------------------------------------------------
// Benchmark (author)            Time             CPU   Iterations
// ---------------------------------------------------------------
// bm_linear_search/64        17.9 ns         17.9 ns     38143169
// bm_linear_search/128       44.3 ns         44.2 ns     15521161
// bm_linear_search/256       74.8 ns         74.7 ns      8836955
