#include <iostream>
#include <thread>

int n = 100000;

void increase(std::size_t &x) {
    for (int i = 0; i != n; ++i) { ++x; }
}

void decrease(std::size_t &x) {
    for (int i = 0; i != n; ++i) { --x; }
}

void increase_multi(std::size_t &x, std::mutex &m) {
    std::lock_guard<std::mutex> guard(m);
    for (int i = 0; i != n; ++i) { ++x; }
}

void decrease_multi(std::size_t &x, std::mutex &m) {
    std::lock_guard<std::mutex> guard(m);
    for (int i = 0; i != n; ++i) { --x; }
}

static void bm_serial(benchmark::State &state) {
    std::size_t x = 0;
    for (auto _ : state) {
        increase(x);
        decrease(x);
        benchmark::DoNotOptimize(x);
    } std::cout << "x: " << x << '\n';
} BENCHMARK(bm_serial);

static void bm_data_race(benchmark::State &state) {
    std::size_t x = 0;
    for (auto _ : state) {
        std::thread t01( [&x] () { increase(x); } );
        std::thread t02( [&x] () { increase(x); } );
        std::thread t03( [&x] () { increase(x); } );
        std::thread t04( [&x] () { increase(x); } );
        
        std::thread t05( [&x] () { decrease(x); } );
        std::thread t06( [&x] () { decrease(x); } );
        std::thread t07( [&x] () { decrease(x); } );
        std::thread t08( [&x] () { decrease(x); } );

        t01.join();
        t02.join();
        t03.join();
        t04.join();
        t05.join();
        t06.join();
        t07.join();
        t08.join();
        
        benchmark::DoNotOptimize(x);
    } std::cout << "x: " << x << '\n';
} BENCHMARK(bm_data_race);

static void bm_no_data_race(benchmark::State &state) {
    std::size_t x = 0;
    std::mutex mu;
    for (auto _ : state) {
        std::thread t01( [&] () { increase_multi(x, mu); } );
        std::thread t02( [&] () { increase_multi(x, mu); } );
        std::thread t03( [&] () { increase_multi(x, mu); } );
        std::thread t04( [&] () { increase_multi(x, mu); } );
        
        std::thread t05( [&] () { decrease_multi(x, mu); } );
        std::thread t06( [&] () { decrease_multi(x, mu); } );
        std::thread t07( [&] () { decrease_multi(x, mu); } );
        std::thread t08( [&] () { decrease_multi(x, mu); } );
        
        t01.join();
        t02.join();
        t03.join();
        t04.join();
        t05.join();
        t06.join();
        t07.join();
        t08.join();
        
        benchmark::DoNotOptimize(x);
    } std::cout << "x: " << x << '\n';
} BENCHMARK(bm_no_data_race);
