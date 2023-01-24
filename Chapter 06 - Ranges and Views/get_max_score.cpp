#include <vector>
#include <string>
#include <algorithm>

#include <benchmark/benchmark.h>

struct Student {
    int year_;
    int score_;
    std::string name_;
};

std::vector<Student> students = {
    {3, 120, "Niki"},
    {2, 140, "Karo"},
    {3, 190, "Sirius"},
    {2, 110, "Rani"},
};

int year = 2;

// so wasteful!
// creats copies of students and year [=]...
// ..., creates ANOTHER vector, then iterates through it again to populate it...
// ..., then returns a dereference to an iterator through a ternary as the answer
// perhaps for the purpose of showing features, but needlessly wasteful
int get_max_score(const std::vector<Student> &students, int year) {
    auto by_year = [=] (const auto &s) { return s.year_ == year; };
    std ::vector<Student> v;
    // std::ranges::copy_if(students, std::back_inserter(v), by_year);
    std::copy_if(students.begin(), students.end(), std::back_inserter(v), by_year);
    // auto it = std::ranges::max_element(v, std::less, &Student::score_);
    auto it = std::max_element(v.begin(), v.end(),
                               [] (const Student &lhs, const Student &rhs) { return lhs.score_ < rhs.score_; } );
    return it != v.end() ? it->score_ : 0;
}

// one pass, no extra space, 4 byte integer fallback to '0' if no solution
int get_max_score_faster(const std::vector<Student> &students, int year) {
    int result = 0;
    
    // ranges doesn't seem to support std::execution::par
    // std::ranges::for_each(students, [&result, year] (const Student &s) {
    std::for_each(students.begin(), students.end(), [&result, year] (const Student &s) {
        if (s.year_ == year) { result = std::max(result, s.score_); }
    } );
    
    return result;
}

static void bm_waste(benchmark::State &state) {
    std::int64_t n = state.range(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(get_max_score(students, year));
    } state.SetComplexityN(n);
}

static void bm_no_waste(benchmark::State &state) {
    std::int64_t n = state.range(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(get_max_score_faster(students, year));
    } state.SetComplexityN(n);
}

BENCHMARK(bm_waste)->RangeMultiplier(2)->Range(64, 4096)->Complexity();
BENCHMARK(bm_no_waste)->RangeMultiplier(2)->Range(64, 4096)->Complexity();

BENCHMARK_MAIN();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// literally 20x faster on QuickBench with std::execution::par in std::for_each
// https://quick-bench.com/q/f_V8X0AWM10cMpakY2gQmF86po8

// Run on (8 X 2700 MHz CPU s)
// CPU Caches:
//   L1 Data 32 KiB
//   L1 Instruction 32 KiB
//   L2 Unified 256 KiB (x4)
//   L3 Unified 6144 KiB
// Load Average: 1.78, 1.61, 1.70
// -----------------------------------------------------------
// Benchmark                 Time             CPU   Iterations
// -----------------------------------------------------------
// bm_waste/64             970 ns          966 ns       707342
// bm_waste/128            962 ns          959 ns       724248
// bm_waste/256            920 ns          917 ns       755417
// bm_waste/512            938 ns          936 ns       780797
// bm_waste/1024           952 ns          949 ns       777890
// bm_waste/2048           960 ns          957 ns       775933
// bm_waste/4096           956 ns          954 ns       761532
// bm_waste_BigO        951.42 (1)      948.36 (1)  
// bm_waste_RMS              2 %             2 %    
// bm_no_waste/64          121 ns          121 ns      5644979
// bm_no_waste/128         119 ns          119 ns      5834597
// bm_no_waste/256         118 ns          118 ns      5891859
// bm_no_waste/512         119 ns          119 ns      5996128
// bm_no_waste/1024        119 ns          119 ns      5560896
// bm_no_waste/2048        121 ns          121 ns      5892256
// bm_no_waste/4096        120 ns          119 ns      5555467
// bm_no_waste_BigO     119.54 (1)      119.30 (1)  
// bm_no_waste_RMS           1 %             1 %    
// Program ended with exit code: 0
