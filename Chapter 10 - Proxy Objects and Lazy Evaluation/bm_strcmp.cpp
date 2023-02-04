#include <string>

#include <benchmark/benchmark.h>

std::string a = "Cole";
std::string b = "Porter";
std::string c = "ColePorter";

bool compare_temp(const std::string &a, const std::string &b, const std::string &c) {
    return (a + b) == c;
}

bool compare_proxy(const std::string &a, const std::string &b, const std::string &c) {
    return a.size() + b.size() == c.size() &&
           std::equal(a.begin(), a.end(), c.begin()) &&
           std::equal(b.begin(), b.end(), c.begin() + a.size());
}

static void bm_temp(benchmark::State &state) {
    for (auto _ : state) {
        compare_temp(a, b, c);
    } state.SetComplexityN(state.range(0));
}

static void bm_proxy(benchmark::State &state) {
    for (auto _ : state) {
        compare_proxy(a, b, c);
    } state.SetComplexityN(state.range(0));
}

BENCHMARK(bm_temp)->RangeMultiplier(2)->Range(1024, 4096)->Complexity();
BENCHMARK(bm_proxy)->RangeMultiplier(2)->Range(1024, 4096)->Complexity();

BENCHMARK_MAIN();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Run on (12 X 24.0026 MHz CPU s)
// CPU Caches:
//   L1 Data 64 KiB
//   L1 Instruction 128 KiB
//   L2 Unified 4096 KiB (x12)
// Load Average: 1.09, 1.44, 1.34
// --------------------------------------------------------
// Benchmark              Time             CPU   Iterations
// --------------------------------------------------------
// bm_temp/1024        90.7 ns         90.7 ns      6525529
// bm_temp/2048        90.9 ns         90.9 ns      7571906
// bm_temp/4096        90.3 ns         90.3 ns      7754772
// bm_temp_BigO       90.64 (1)       90.64 (1)
// bm_temp_RMS            0 %             0 %
// bm_proxy/1024        170 ns          170 ns      4122376
// bm_proxy/2048        170 ns          170 ns      4119537
// bm_proxy/4096        170 ns          170 ns      4130598
// bm_proxy_BigO     169.84 (1)      169.84 (1)
// bm_proxy_RMS           0 %             0 %
// Program ended with exit code: 0
