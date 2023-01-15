#include <vector>
#include <iostream>

// the book mentions using iterators, so we'll give it a go
std::vector<int>::const_iterator binary_search(const std::vector<int> &vals, const int &key) {
    if (key < vals.front() || key > vals.back()) { return vals.end(); }
    
    auto lo = vals.begin(), hi = vals.end() - 1;
    while (lo <= hi) {
        auto mid = lo;
        std::advance(mid, (hi - lo) / 2);
        
        if (key == *mid) { return mid; }
        if (key < *mid)  { hi = mid - 1; }
        else             { lo = mid + 1; }
    }
    
    return vals.end();
}

// original STL implementation
// template<class ForwardIt, class T>
// bool stl_binary_search(ForwardIt first, ForwardIt last, const T& value)
// {
//     first = std::lower_bound(first, last, value);
//     return (!(first == last) and !(value < *first));
// }

// STL tweaked to return an iterator
// it will return a slightly difference asnwers if there are duplicates in the array
template<class ForwardIt, class T>
ForwardIt stl_binary_search(ForwardIt first, ForwardIt last, const T& value)
{
    first = std::lower_bound(first, last, value);
    return (!(first == last) and !(value < *first)) ? first : last;
}

int main()
{
    std::vector<int> ivec = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int key = 7;
    
    std::cout << "\nsearching for " << key << " with my binary search...\n";
    binary_search(ivec, key) != ivec.end()
        ? std::cout << key << " was found in vals[" << binary_search(ivec, key) - ivec.begin() << "]\n"
        : std::cout << key << " was not found.\n";
    
    std::cout << "\nsearching for " << key << " with a tweaked STL binary search...\n";
    stl_binary_search(ivec.begin(), ivec.end(), key) != ivec.end()
        ? std::cout << key << " was found in vals[" << stl_binary_search(ivec.begin(), ivec.end(), key) - ivec.begin() << "]\n"
        : std::cout << key << " was not found.\n";
    
    return 0;
}
