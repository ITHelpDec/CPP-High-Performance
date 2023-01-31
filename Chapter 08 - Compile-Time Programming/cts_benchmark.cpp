#include <iostream>

#include <benchmark/benchmark.h>

constexpr std::size_t hash_function(const char* str) {
    std::size_t sum = 0;
    for (auto ptr = str; *ptr != '\0'; ++ptr) {
        // std::cout << "char: " << *ptr << ", sum: " << sum << '\n';
        sum += *ptr;
    }
    return sum;
}

std::size_t hash_string(const std::string &s) {
    std::size_t sum = 0;
    for (const auto &c : s) { sum += c; }
    return sum;
}

std::size_t hash_string(std::string_view s) {
    std::size_t sum = 0;
    for (auto c : s) { sum += c; }
    return sum;
}

static void bm_cstr(benchmark::State &state) {
    auto n = state.range(0);
    const char *str = "asdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdb!";
    for (auto _ : state) {
        hash_function(str);
        benchmark::DoNotOptimize(str);
    } state.SetComplexityN(n);
}

static void bm_string(benchmark::State &state) {
    auto n = state.range(0);
    std::string s = "asdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdb!";
    for (auto _ : state) {
        hash_string(s);
        benchmark::DoNotOptimize(s);
    } state.SetComplexityN(n);
}

static void bm_sview(benchmark::State &state) {
    auto n = state.range(0);
    std::string_view s_view = "asdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdbasdbasjkd78wqevsakbdsJASKJDbbuq97dbiausdb!";
    for (auto _ : state) {
        hash_string(s_view);
        benchmark::DoNotOptimize(s_view);
    } state.SetComplexityN(n);
}

BENCHMARK(bm_cstr)->Complexity()->RangeMultiplier(2)->Range(1024, 4096);
BENCHMARK(bm_string)->Complexity()->RangeMultiplier(2)->Range(1024, 4096);
BENCHMARK(bm_sview)->Complexity()->RangeMultiplier(2)->Range(1024, 4096);

BENCHMARK_MAIN();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 2023-01-31T21:17:01+00:00
// Run on (8 X 2700 MHz CPU s)
// CPU Caches:
//   L1 Data 32 KiB
//   L1 Instruction 32 KiB
//   L2 Unified 256 KiB (x4)
//   L3 Unified 6144 KiB
// Load Average: 1.47, 1.68, 1.78
// ---------------------------------------------------------
// Benchmark               Time             CPU   Iterations
// ---------------------------------------------------------
// bm_cstr/1024         1274 ns         1265 ns       539146
// bm_cstr/2048         1271 ns         1268 ns       529938
// bm_cstr/4096         1255 ns         1254 ns       537899
// bm_cstr_BigO      1266.51 (1)     1262.60 (1)  
// bm_cstr_RMS             1 %             0 %    
// bm_string/1024       6774 ns         6768 ns        97197
// bm_string/2048       6663 ns         6658 ns       104370
// bm_string/4096       6855 ns         6843 ns       102184
// bm_string_BigO    6763.90 (1)     6756.35 (1)  
// bm_string_RMS           1 %             1 %    
// bm_sview/1024        1490 ns         1489 ns       446141
// bm_sview/2048        1503 ns         1501 ns       466396
// bm_sview/4096        1510 ns         1509 ns       472239
// bm_sview_BigO     1500.90 (1)     1499.87 (1)  
// bm_sview_RMS            1 %             1 %    
// Program ended with exit code: 0
