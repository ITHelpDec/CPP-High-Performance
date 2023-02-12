#include <iostream>
#include <thread>
#include <vector>

struct Element {
    int e = 42;
};

struct alignas(_X86_INSTRUCTION_STATE_CACHELINE_SIZE) AlignedElement {
    int e = 42;
};

template <typename E>
void printMemoryLocations(const std::vector<E> &evec) {
    int i = 1;
    for (const auto &e : evec) {
        std::cout << i << ": " << &e << " (" << sizeof(e) << " bytes wide)\n";
        ++i;
    } std::cout << '\n';
}

int main()
{
    std::size_t num_threads = std::thread::hardware_concurrency();
    
    std::vector<Element> evec1(num_threads);
    std::vector<AlignedElement> evec2(num_threads);
    
    std::cout << "Element Vector\n";
    printMemoryLocations(evec1);
    
    std::cout << "AlignedElement Vector\n";
    printMemoryLocations(evec2);
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Element Vector
// 1: 0x6000002052c0 (4 bytes)
// 2: 0x6000002052c4 (4 bytes)
// 3: 0x6000002052c8 (4 bytes)
// 4: 0x6000002052cc (4 bytes)
// 5: 0x6000002052d0 (4 bytes)
// 6: 0x6000002052d4 (4 bytes)
// 7: 0x6000002052d8 (4 bytes)
// 8: 0x6000002052dc (4 bytes)

// AlignedElement Vector
// 1: 0x1005044c0 (64 bytes)
// 2: 0x100504500 (64 bytes)
// 3: 0x100504540 (64 bytes)
// 4: 0x100504580 (64 bytes)
// 5: 0x1005045c0 (64 bytes)
// 6: 0x100504600 (64 bytes)
// 7: 0x100504640 (64 bytes)
// 8: 0x100504680 (64 bytes)

// Program ended with exit code: 0
