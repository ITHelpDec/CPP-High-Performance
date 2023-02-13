// #include "resumable.h"
#include <iostream>

struct LambdaStruct {
    // same as [] () {}, but a member function
    Resumable operator() (int a) {
        std::cout << a << '\n';
        co_return;
    }
};

int main()
{
    // keep alive!
    LambdaStruct lambda;
    Resumable coro = lambda(42);
    coro.resume();
    
    // standalone lambda
    // [X] Resumable innerlambda = [] (int a) {
    // [X] auto innerlambda = [] (int a) {
    auto innerlambda = [] (int a) -> Resumable {
        std::cout << a << '\n';
        co_return;
    };
    
    innerlambda(42).resume();
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 42
// 42
// Program ended with exit code: 0
