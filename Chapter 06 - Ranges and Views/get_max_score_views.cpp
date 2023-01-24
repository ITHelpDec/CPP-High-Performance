#include <vector>
#include <string>
#include <algorithm>
#include <ranges>

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

int max_value(auto &&range) {
    const auto it = std::ranges::max_element(range);
    return it != range.end() ? * it : 0;
}

int get_max_score(const std::vector<Student> &students, int year) {
    auto by_year = [=] (auto &&s) { return s.year_ == year; };
    return max_value(students
                     | std::views::filter(by_year)
                     | std::views::transform(&Student::score_));
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
// BENCHMARKS  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// better, but still not as goos as for_each
// https://quick-bench.com/q/DKdnRx0EfqRJ8Yp4l3yyZ8MtL8o
