#include <string>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <numeric>

#include <benchmark/benchmark.h>

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

auto setup_fixture(int n) {
    std::vector<float> src(n);
    std::iota(src.begin(), src.end(), 1.0f); // Values from 1.0 to n
    
    std::vector<float> dst(src.size());
    
    auto transform_function = [] (float v) {
        auto sum = v;
        for (auto i = 0; i < 500; ++i) {
            sum += (i * i * i * sum);
        } return sum;
    };
    
    return std::tuple{src, dst, transform_function};
}

void bm_sequential(benchmark::State& state) {
    auto [src, dst, f] = setup_fixture(state.range(0));
    for (auto _ : state) {
        std::transform(src.begin(), src.end(), dst.begin(), f);
    }
}

void bm_parallel(benchmark::State& state) {
    auto [src, dst, f] = setup_fixture(state.range(0));
    for (auto _ : state) {
        naive_par_transform(src.begin(), src.end(), dst.begin(), f);
    }
}

void CustomArguments(benchmark::internal::Benchmark* b) {
    b->Arg(50)->Arg(10'000)->Arg(1'000'000) // Input size
    ->MeasureProcessCPUTime()               // Measure all threads
    ->UseRealTime()                         // Clock on the wall
    ->Unit(benchmark::kMillisecond);        // Use ms
}

BENCHMARK(bm_sequential)->Apply(CustomArguments);
BENCHMARK(bm_parallel)->Apply(CustomArguments);
BENCHMARK_MAIN();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Run on (12 X 24.0252 MHz CPU s)
// CPU Caches:
//   L1 Data 64 KiB
//   L1 Instruction 128 KiB
//   L2 Unified 4096 KiB (x12)
// Load Average: 1.16, 1.19, 1.41
// ---------------------------------------------------------------------------------------
// Benchmark                                             Time             CPU   Iterations
// ---------------------------------------------------------------------------------------
// bm_sequential/50/process_time/real_time           0.083 ms        0.083 ms         6909
// bm_sequential/10000/process_time/real_time         16.4 ms         16.4 ms           43
// bm_sequential/1000000/process_time/real_time       1651 ms         1651 ms            1
// bm_parallel/50/process_time/real_time             0.088 ms        0.270 ms         7246
// bm_parallel/10000/process_time/real_time           2.15 ms         20.1 ms          346
// bm_parallel/1000000/process_time/real_time          170 ms         1972 ms            4
// Program ended with exit code: 0
