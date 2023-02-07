#include <random>
#include <iostream>
#include <array>
#include <vector>
#include <thread>
#include <barrier>

int random_int()
{   // thread_local helps create our static variable once per thread
    static thread_local std::default_random_engine e(std::random_device{}());
    static std::uniform_int_distribution u(1, 6);
    return u(e);
}

int main()
{
    constexpr std::size_t n_dice = 5;
    std::size_t n_turns = 0;
    
    bool done = false;
    
    std::array<int, n_dice> dice;
    std::vector<std::thread> threads;
    
    auto bar = std::barrier(n_dice, [&] () {
        ++n_turns;
        done = std::all_of(dice.begin(), dice.end(), [] (int x) { return x == 6; } );
    } );
    
    for (int i = 0; i != n_dice; ++i) {
        threads.emplace_back( [&, i] () {
            while (!done) {
                dice[i] = random_int();
                bar.arrive_and_wait();
            }
        } );
    }
    
    for (auto &&thread : threads) { thread. join(); }
    
    std::cout << n_turns << (n_turns == 1 ? " turn\n" : " turns\n");
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 11675 turns
// 2163 turns
// 101 turns
// ...
// Program ended with exit code: 0
