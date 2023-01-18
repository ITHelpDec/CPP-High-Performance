# C++ High Performance (2nd Edition)

## Highlights from Chapter 4 - "Data Structures"

### Latency numbers every programmer should know

| Reference             | Latency  |
| :--- | --- |
| L1 cache reference    |   0.5 ns |
| L2 cache reference    |   7   ns |
| Main memory reference | 100   ns |

* Temporal locality: Accessing data that has recently been used (like dynamic programming)
* Spatial locality: Accessing data near some other data you are using
* Cache thrashing: Constantly wiping out the cache lines in the likes of inner loops
#
### Moral of the story
Organise your memory contiguously
#
[loop_interchange.cpp](loop_interchange.cpp)
#
### Additions to std::vector from C++20
```cpp
std::vector<int> ivec = { -1, 5, 2, -3, 4, -5, 5 };
std::erase(v, 5);
std::erase_if(v, [] (const int &x) { return x < 0; } );
```
[vector.cpp](vector.cpp)
