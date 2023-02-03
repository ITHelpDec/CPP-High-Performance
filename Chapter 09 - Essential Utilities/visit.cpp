#include <variant>
#include <iostream>

int main()
{
    std::variant<int, bool, const char*> v = "hello";
    std::visit( [] (auto&& e) { std::cout << e << '\n'; } , v);
    
    v = false;
    std::visit( [] (auto&& e) { std::cout << e << '\n'; } , v);
    
    v = "woof";
    std::visit( [] (auto&& e) { std::cout << e << '\n'; } , v);
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// hello
// 0
// woof
// Program ended with exit code: 0
