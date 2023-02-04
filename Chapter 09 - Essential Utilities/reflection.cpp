#include <iostream>

class Player {
public:
    Player(const char* name, int level, int score)
        : name_(name), level_(level), score_(score) { }
    
    auto reflect() const { return std::tie(name_, level_, score_ ); }
    
private:
    const char* name_;
    int level_, score_;
};

int main()
{
    auto t = Player("woof", 1, 2).reflect();
    
    std::apply([] (auto&& ...e) { ( (std::cout << e << ' '), ...) << '\n'; }, t);
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// woof 1 2 
// Program ended with exit code: 0
