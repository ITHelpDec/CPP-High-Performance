#include <iostream>
#include <type_traits>

#include <benchmark/benchmark.h>

struct AnimalBase {
    virtual ~AnimalBase() { }
    virtual void speak() const { }
};

struct BearDerived : public AnimalBase {
    virtual void speak() const override { roar(); }
    void roar() const { }
};

struct DogDerived : public AnimalBase {
    virtual void speak() const override { woof(); }
    void woof() const { }
};

void speakDerived(const AnimalBase &a) {
    a.speak();
}

struct BearConstexpr {
    void roar() const { }
};

struct DogConstexpr {
    void bark() const { }
};

// _"the compiler will keep the call to the member function, quack(), ...
// ...hich will then fail to compile since Bear does not contain a quack() member function"_
template <typename Animal> void speakConstexpr(const Animal &a) {
    // if (std::is_same_v<Animal, Bear>) {
    if constexpr (std::is_same_v<Animal, BearConstexpr>) {
        a.roar();
        // } else if (std::is_same_v<Animal, Dog>){
    } else if constexpr (std::is_same_v<Animal, DogConstexpr>){
        a.bark();
        // } else { // finds our issues at compile-time
        //     static_assert(false, "not an animal\n");;
    }
}

static void bm_polymorphic(benchmark::State &state) {
    auto n = state.range(0);
    for (auto _ : state) {
        BearDerived bear;
        speakDerived(bear);
        benchmark::DoNotOptimize(bear);
        
        DogDerived dog;
        speakDerived(dog);
        benchmark::DoNotOptimize(dog);
    } state.SetComplexityN(n);
}

static void bm_constexpr(benchmark::State &state) {
    auto n = state.range(0);
    for (auto _ : state) {
        BearConstexpr bear;
        speakConstexpr(bear);
        benchmark::DoNotOptimize(bear);
        
        DogConstexpr dog;
        speakConstexpr(dog);
        benchmark::DoNotOptimize(dog);
    } state.SetComplexityN(n);
}

BENCHMARK(bm_polymorphic)->Complexity()->RangeMultiplier(2)->Range(1024, 4096);
BENCHMARK(bm_constexpr)->Complexity()->RangeMultiplier(2)->Range(1024, 4096);

BENCHMARK_MAIN();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Run on (8 X 2700 MHz CPU s)
// CPU Caches:
// L1 Data 32 KiB
// L1 Instruction 32 KiB
// L2 Unified 256 KiB (x4)
// L3 Unified 6144 KiB
// Load Average: 2.05, 1.33, 1.30
// --------------------------------------------------------------
// Benchmark                    Time             CPU   Iterations
// --------------------------------------------------------------
// bm_polymorphic/1024       31.1 ns         31.0 ns     21552189
// bm_polymorphic/2048       31.2 ns         31.1 ns     22689923
// bm_polymorphic/4096       31.1 ns         30.9 ns     22917458
// bm_polymorphic_BigO      31.14 (1)       30.98 (1)
// bm_polymorphic_RMS           0 %             0 %
// bm_constexpr/1024         10.3 ns         10.3 ns     65141124
// bm_constexpr/2048         10.4 ns         10.4 ns     69697513
// bm_constexpr/4096         10.4 ns         10.4 ns     70826546
// bm_constexpr_BigO        10.38 (1)       10.35 (1)
// bm_constexpr_RMS             0 %             0 %
// Program ended with exit code: 0
