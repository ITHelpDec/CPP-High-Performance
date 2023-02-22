#include <benchmark/benchmark.h>

#include <iostream>
#include <algorithm>
#include <future>
#include <string>
#include <numeric>
#include <tuple>
#include <vector>

template <class _InputIt, class _OutputIt, class _UnaryOperation>
_OutputIt naive_par_transform(_InputIt __first, _InputIt __last, _OutputIt __result, _UnaryOperation __op) {
    std::size_t n = static_cast<std::size_t>(std::distance(__first, __last));
    std::size_t n_cores = std::thread::hardware_concurrency();
    std::size_t n_tasks = std::max(n_cores, std::size_t(1));
    std::size_t chunk_sz = (n + n_tasks - 1) / n_tasks;
    
    std::vector<std::future<void>> futures;
    
    for (auto i = 0ul; i != n_tasks; ++i) {
        auto start = chunk_sz * i;
        if (start < n) {
            auto stop = std::min(chunk_sz * (i + 1), n);
            auto fut = std::async(std::launch::async, [__first, __result, start, stop, __op] () {
                std::transform(__first + start, __first + stop, __result + start, __op);
            });
            futures.emplace_back(std::move(fut));
        }
    }
    
    for (auto &&fut : futures) { fut. wait(); }
    
    return __result;
}

template <class _InputIt, class _OutputIt, class _UnaryOperation>
_OutputIt par_transform(_InputIt __first, _InputIt __last, _OutputIt __result, _UnaryOperation __op, std::size_t chunk) {
    const std::size_t n = static_cast<std::size_t>(std::distance(__first, __last));

    if (n <= chunk) {
        std::transform(__first, __last, __result, __op);
        return __result;
    }

    const _InputIt __middle_in = std::next(__first, n / 2);

    auto future = std::async(std::launch::async, [=, &__op] () {
        par_transform(__first, __middle_in, __result, __op, chunk);
    });

    const _OutputIt __middle_out = std::next(__result, n / 2);

    par_transform(__middle_in, __last, __middle_out, __op, chunk);

    future.wait();

    return __result;
}

// avoid implicit conversion
// auto setup_fixture(int n) {
auto setup_fixture(int64_t n) {
    std::vector<float> src(n);
    std::iota(src.begin(), src.end(), 1.0f);
    
    auto dst = std::vector<float>(src.size());
    
    auto transform_function = [](float v) {
        auto sum = v;
        auto n = v / 20'000;
        for (auto i = 0; i < n; ++i) {
            sum += (i * i * i * sum);
        } return sum;
    };
    
    return std::tuple{src, dst, transform_function};
}

// Divide and conquer version
void bm_parallel(benchmark::State& state) {
    auto [src, dst, f] = setup_fixture(10'000'000);
    auto n = state.range(0); // Chunk size is parameterized
    for (auto _ : state) {
        par_transform(src.begin(), src.end(), dst.begin(), f, n);
    }
}

// Naive version
void bm_parallel_naive(benchmark::State& state) {
    auto [src, dst, f] = setup_fixture(10'000'000);
    for (auto _ : state) {
        naive_par_transform(src.begin(), src.end(), dst.begin(), f);
    }
}
void CustomArguments(benchmark::internal::Benchmark* b) {
    b->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);
}

BENCHMARK(bm_parallel)->Apply(CustomArguments)
    ->RangeMultiplier(10) // Chunk size goes from
    ->Range(1000, 10'000'000); // 1k to 10M

BENCHMARK(bm_parallel_naive)->Apply(CustomArguments);

BENCHMARK_MAIN();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Run on (12 X 24.1206 MHz CPU s)
// CPU Caches:
//   L1 Data 64 KiB
//   L1 Instruction 128 KiB
//   L2 Unified 4096 KiB (x12)
// Load Average: 2.12, 1.89, 2.01
// --------------------------------------------------------------------------------------
// Benchmark                                            Time             CPU   Iterations
// --------------------------------------------------------------------------------------
// bm_parallel/1000/process_time/real_time           1612 ms        12201 ms            1
// bm_parallel/10000/process_time/real_time           848 ms         9960 ms            1
// bm_parallel/100000/process_time/real_time          868 ms         9856 ms            1
// bm_parallel/1000000/process_time/real_time        1213 ms         9560 ms            1
// bm_parallel/10000000/process_time/real_time       8127 ms         8127 ms            1
// bm_parallel_naive/process_time/real_time          1470 ms         9329 ms            1
// Program ended with exit code: 0
