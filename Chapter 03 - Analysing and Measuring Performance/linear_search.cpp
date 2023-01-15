#include <vector>
#include <iostream>

// I feel an iterator would be more useful than a bool
// we'd then need to remove the noexcept to avoid throwing an exception by dereferencing a nullptr
// bool linear_search(const std::vector<int>& vals, int key) noexcept {
std::vector<int>::const_iterator linear_search(const std::vector<int> &vals, const int &key) {
    for (auto it = vals.begin(); it != vals.end(); ++it) {
        if (*it == key) { return it; }
    } return vals.end();
}

// STL implementation
template<class InputIt, class T>
constexpr InputIt stl_find(InputIt first, InputIt last, const T& value)
{
    for (; first != last; ++first)
        if (*first == value)
            return first;
    
    return last;
}

int main()
{
    std::vector<int> ivec = { 1, 2, 3, 4, 5, 6 };
    int key = 5;
    
    std::cout << "searching for " << key << " with my linear search...\n";
    linear_search(ivec, key) != ivec.end()
        ? std::cout << key << " was found in vals[" << linear_search(ivec, key) - ivec.begin() << "]\n"
        : std::cout << key << " was not found.\n";
    
    std::cout << "\nsearching for " << key << " with STL find...\n";
    stl_find(ivec.begin(), ivec.end(), key) != ivec.end()
        ? std::cout << key << " was found in vals[" << stl_find(ivec.begin(), ivec.end(), key) - ivec.begin() << "]\n"
        : std::cout << key << " was not found.\n";
    
    return 0;
}
