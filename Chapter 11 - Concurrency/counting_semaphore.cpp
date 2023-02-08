#include <semaphore>
#include <thread>
#include <iostream>

class Request { };

class Server {
public:
    // Server() : sem_(4) { }
    
    void handle(const Request &req)
    {
        sem_.acquire();
        do_handle(req);
        sem_.release();
    }
    
private:
    std::counting_semaphore<4> sem_{4};
    // or
    // std::counting_semaphore<4> sem_ = std::counting_semaphore<4>(4);
    // or
    // std::counting_semaphore<4> sem_; + constructor
    
    void do_handle(const Request &req) { std::cout << "woofwoof\n"; }
};

int main()
{
    Server().handle(Request());
  
    return 0;
}
