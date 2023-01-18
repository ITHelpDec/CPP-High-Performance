#include <vector>
#include <benchmark/benchmark.h>

constexpr auto l1_cache = 512;
constexpr auto k = l1_cache / sizeof(int);

// unfortunately, DoNotOptimize() does not have a valid constructor for this function
// benchmark::DoNotOptimize(cache_thrashing(matrix, counter));
// No matching function for call to 'DoNotOptimize'
void cache_thrashing(std::vector<std::vector<int>> &matrix, int &counter) {
    for (int i = 0; i != k; ++i) {
        for (int j = 0; j != k; ++j) {
            matrix[i][j] = ++counter;
        }
    }
}

static void bm_cache_thrash(benchmark::State &state) {
    std::vector<std::vector<int>> matrix(k, std::vector<int>(k));
    int counter = 0;
    for (int i = 0; i != k; ++i)
        for (int j = 0; j != k; ++j)
            for (auto _ : state)
                benchmark::DoNotOptimize(matrix[i][j] = ++counter);
}

static void bm_no_cache_thrash(benchmark::State &state) {
    std::vector<std::vector<int>> matrix(k, std::vector<int>(k));
    int counter = 0;
    for (int i = 0; i != k; ++i)
        for (int j = 0; j != k; ++j)
            for (auto _ : state)
                benchmark::DoNotOptimize(matrix[j][i] = ++counter);
}

BENCHMARK(bm_cache_thrash);
BENCHMARK(bm_no_cache_thrash);

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
// Load Average: 1.59, 1.98, 1.92
// -------------------------------------------------------------
// Benchmark                   Time             CPU   Iterations
// -------------------------------------------------------------
// bm_cache_thrash        175928 ns       173807 ns         3801
// bm_no_cache_thrash     225197 ns       224755 ns         3165
// Program ended with exit code: 0
