#include <vector>
#include <iostream>

template <typename T> void printContainer(const T& t) {
    for (const auto &e : t) {
        std::cout << e << " ";
    } std::cout << std::endl;
}

int main()
{
    std::vector<int> ivec = { -1, 5, 2, -3, 4, -5, 5 };
    printContainer(ivec);
    
    std::erase(ivec, 5);
    printContainer(ivec);
    
    std::erase_if(ivec, [] (const int &x) { return x < 0; } );
    printContainer(ivec);
    
    return 0;
}

// output:
// -1 5 2 -3 4 -5 5 
// -1 2 -3 4 -5 
// 2 4 
// Program ended with exit code: 0
