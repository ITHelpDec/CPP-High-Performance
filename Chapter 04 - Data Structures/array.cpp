#include <array>

void f(const std::array<int, 1024> &stl_arr) { /* do stuff */ }
void g(const int c_arr[]) { /* do stuff */ }

int main()
{
    std::array<int, 16> stl_arr;
    int c_arr[16];
    
    f(stl_arr);  // will not compile
    g(c_arr);    // will compile, but unsafe (array decay)
    
    return 0;
}
