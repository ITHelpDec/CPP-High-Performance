#include <benchmark/benchmark.h>

#include <string>
#include <random>

bool bool_state() {
    static std::default_random_engine e;
    static std::bernoulli_distribution u;
    return u(e);
}

bool compare_temp(const std::string &a, const std::string &b, const std::string &c) {
    return (a + b) == c;
}

bool compare_proxy(const std::string &a, const std::string &b, const std::string &c) {
    return a.size() + b.size() == c.size() &&
           std::equal(a.begin(), a.end(), c.begin()) &&
           std::equal(b.begin(), b.end(), c.begin() + a.size());
}

static void bm_temp(benchmark::State &state) {
    auto n = state.range(0);
    for (auto _ : state) {
        bool result = bool_state();
        
        std::string a = "Cole";
        std::string b = "Porter";
        std::string c = "ColePorter";

        result = compare_temp(a, b, c);
        
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
        benchmark::DoNotOptimize(c);
        benchmark::DoNotOptimize(result);
    } state.SetComplexityN(n);
}

static void bm_proxy(benchmark::State &state) {
    auto n = state.range(0);
    for (auto _ : state) {
        bool result = bool_state();
        
        std::string a = "Cole";
        std::string b = "Porter";
        std::string c = "ColePorter";

        result = compare_proxy(a, b, c);
        
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
        benchmark::DoNotOptimize(c);
        benchmark::DoNotOptimize(result);
    } state.SetComplexityN(n);
}

BENCHMARK(bm_temp)->RangeMultiplier(2)->Range(1024, 4096)->Complexity();
BENCHMARK(bm_proxy)->RangeMultiplier(2)->Range(1024, 4096)->Complexity();

BENCHMARK_MAIN();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Run on (12 X 24.0647 MHz CPU s)
// CPU Caches:
//   L1 Data 64 KiB
//   L1 Instruction 128 KiB
//   L2 Unified 4096 KiB (x12)
// Load Average: 1.71, 1.29, 1.21
// --------------------------------------------------------
// Benchmark              Time             CPU   Iterations
// --------------------------------------------------------
// bm_temp/1024         189 ns          189 ns      3693756
// bm_temp/2048         189 ns          189 ns      3720594
// bm_temp/4096         189 ns          189 ns      3713154
// bm_temp_BigO      188.86 (1)      188.85 (1)
// bm_temp_RMS            0 %             0 %
// bm_proxy/1024        276 ns          276 ns      2533845
// bm_proxy/2048        277 ns          277 ns      2546427
// bm_proxy/4096        277 ns          277 ns      2536728
// bm_proxy_BigO     276.69 (1)      276.68 (1)
// bm_proxy_RMS           0 %             0 %
// Program ended with exit code: 0
