#include <thread>
#include <queue>
#include <mutex>
#include <iostream>

std::condition_variable cv;
std::queue<int> iq;
std::mutex mtx;
constexpr int sentinel = -1;

void print_ints() {
    int i = 0;
    while (i != sentinel) {
        
        {
            std::unique_lock<std::mutex> lock(mtx);
            
            // while (iq.empty()) { cv.wait(lock); }
            cv.wait(lock, [] () { return !iq.empty(); } );
            
            i = iq.front();
            iq.pop();
        }
        
        if (i != sentinel) { std::cout << "Got: " << i << '\n'; }
    }
}

void generate_ints() {
    for (int i : { 1, 2, 3, sentinel } ) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        {
            std::scoped_lock<std::mutex> lock(mtx);
            iq.push(i);
        }
        
        cv.notify_one();
    }
}

int main()
{
    std::thread producer(generate_ints);
    std::thread consumer(print_ints);
    
    producer.join();
    consumer.join();
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Got: 1
// Got: 2
// Got: 3
// Program ended with exit code: 0
