#include <tuple>
#include <iostream>

int main()
{
    std::tuple<bool, double, int> t1;
    std::tuple<bool, int, double> t2;
    std::tuple<int, double, bool> t3;
    std::tuple<int, bool, double> t4;
    std::tuple<double, bool, int> t5;
    std::tuple<double, int, bool> t6;
    
    std::cout << "t1: " << sizeof(t1) << '\n';
    std::cout << "t2: " << sizeof(t2) << '\n';
    std::cout << "t3: " << sizeof(t3) << '\n';
    std::cout << "t4: " << sizeof(t4) << '\n';
    std::cout << "t5: " << sizeof(t5) << '\n';
    std::cout << "t6: " << sizeof(t6) << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//t1: 24
//t2: 16
//t3: 24
//t4: 16
//t5: 16
//t6: 16
//Program ended with exit code: 0
