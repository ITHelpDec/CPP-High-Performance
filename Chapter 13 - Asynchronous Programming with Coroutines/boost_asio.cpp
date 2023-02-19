#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

// for chrono literals
using namespace std::chrono;

int main()
{
    boost::asio::io_context ctx;
    boost::asio::system_timer timer(ctx);
    
    timer.expires_from_now(1000ms);
    timer.async_wait( [] (auto error) {
        std::cout << "Hello from delayed callback\n";
    } );
    
    std::cout << "Hello from main thread...\n";
    ctx.run();
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Hello from main thread...
// Hello from delayed callback
// Program ended with exit code: 0
