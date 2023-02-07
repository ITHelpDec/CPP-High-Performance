#include <thread>
#include <iostream>
#include <stop_token>

void print(std::stop_token stoken) {
    while (!stoken.stop_requested()) {
        std::cout << this_thread::get_id() << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(3));
    } std::cout << "Stop requested." << '\n';
}

int main()
{
    std::jthread jt1(print);
    
    std::cout << "main goes to sleep...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    std::cout << "main requests jthread to stop...\n";
    jt1.request_stop();
    
    return 0;
}
