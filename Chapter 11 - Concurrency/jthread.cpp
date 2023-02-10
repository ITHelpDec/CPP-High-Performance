#include <thread>
#include <iostream>

void print(std::stop_token stoken) {
    while (!stoken.stop_requested()) {
        std::cout << std::this_thread::get_id() << '\n';
        std::this_thread::sleep_for(std::chrono::seconds{1});
    } std::cout << "Stop requested\n";
}

int main() {
    std::jthread = joinable_thread(print);
    
    std::cout << "main: goes to sleep\n";
    std::this_thread::sleep_for(std::chrono::seconds{3});
    
    std::cout << "main: request jthread to stop\n";
    joinable_thread.request_stop();
    
    return 0;
}
