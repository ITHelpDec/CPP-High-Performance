#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <iostream>
#include <string>

using namespace std::chrono;

boost::asio::awaitable<void> serve_client(boost::asio::ip::tcp::socket socket) {
    std::cout << "New client connected\n";
    auto ex = co_await boost::asio::this_coro::executor;
    boost::asio::system_timer timer(ex);
    std::size_t counter = 0;
    while (true) {
        try {
            std::string s = std::to_string(counter) + "\n";
            auto buf = boost::asio::buffer(s.data(), s.size());
            auto n = co_await boost::asio::async_write(socket, buf, boost::asio::use_awaitable);
            std::cout << "Wrote " << n << " byte(s)\n";
            ++counter;
            timer.expires_from_now(100ms);
            co_await timer.async_wait(boost::asio::use_awaitable);
        } catch (...) {
            // Error or client disconnected break;
        }
    }
}

boost::asio::awaitable<void> listen(boost::asio::ip::tcp::endpoint endpoint) {
    auto ex = co_await boost::asio::this_coro::executor;
    boost::asio::ip::tcp::acceptor a(ex, endpoint);
    while (true) {
        auto socket = co_await a.async_accept(boost::asio::use_awaitable);
        auto session = [s = std::move(socket)] () mutable {
            auto awaitable = serve_client(std::move(s));
            return awaitable;
        };
        boost::asio::co_spawn(ex, std::move(session), boost::asio::detached);
    }
}

int main()
{
    auto server = [] () {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 37259);
        auto awaitable = listen(endpoint);
        return awaitable;
    };
    
    boost::asio::io_context ctx;
    boost::asio::co_spawn(ctx, server, boost::asio::detached);
    ctx.run();
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Server side:
// New client connected
// Wrote 2 byte(s)
// Wrote 2 byte(s)
// Wrote 2 byte(s)
// ...

// Client side:
// 0
// 1
// 2
// ...
