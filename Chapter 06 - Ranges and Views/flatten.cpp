#include <vector>
#include <ranges>
#include <iostream>

int main()
{
    std::vector<std::vector<int>> list_of_lists = {
        { 1, 2 },
        { 3, 4, 5 },
        { 5 },
        { 4, 3, 2, 1 }
    };
    
    auto flattened_view = std::views::join(list_of_lists);
    
    for (auto element : flattened_view) {
        std::cout << element << " ";
    } std::cout << std::endl;
    
    return 0;
}

// output:
// 1 2 3 4 5 5 4 3 2 1
