# C++ High Performance (2nd Edition)

## Highlights from Chapter 7 - "Memory Management"

### Stack Memory
<details> 
  <summary>Direction of stack growth</summary>
  
  ```cpp
  #include <iostream>

void f1() {
    int i = 0;
    std::cout << "f1()  : " << std::addressof(i) << '\n';
}

void f2() {
    int i = 0;
    std::cout << "f2()  : " << std::addressof(i) << '\n';
    f1();
}

int main() {
    int i = 0;
    std::cout << "main(): " << std::addressof(i) << '\n';

    f2();
    f1();

    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// main(): 0x7ff7bfeff168
// f2()  : 0x7ff7bfeff14c
// f1()  : 0x7ff7bfeff12c
// f1()  : 0x7ff7bfeff14c
// Program ended with exit code: 0
  ```
  
</details>

<details> 
  <summary>Default stack size</summary>
  
  ```cpp
  #include <iostream>

void func(std::byte *stack_bottom_order) {
    std::byte data[1024];
    
    std::cout << stack_bottom_order - data << '\n';
    
    func(stack_bottom_order);
}

int main()
{
    std::byte b;
    
    func(&b);
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 1067
// 2139
// 3211
// 4283
// ...
// 8378747
// 8379819
// 8380891
// 8381963
  ```
  
</details>

We can view the default stack size on a UNIX-based system using the commands `ulimit -s`
  
It's worth remembering that (of the time the book was published) Windows defaults to 1MB of stack, whereas macOS defaults to 8MB, so something designed using macOS or Linux may unintentionally cause a stack overflow on Windows.
#
### The Heap (or _free store_)
Heap allocations are shared across threads (stack allocations are local per thread)
  
Stack allocations and deallocations are sequential; heap are arbitrary, which can lead to higher fragmentation.
#
### Dynamically allocating memory
<details>
  <summary>Using new (not recommended)</summary>
  
  ```cpp
  class User {
public:
    User(std::string&& name) : name_(name)
    {
        std::cout << "user \"" << name << "\" created" << std::endl;
    }
    
    ~User()
    {
        std::cout << "deleting " << name_ << "..." << std::endl;
    }
    
    void print_name() { std::cout << name_ << std::endl; }
private:
    std::string name_;
};

int main()
{
    User *u1 = new User("John");
    
    u1->print_name();
    
    delete u1;
    u1 = nullptr;
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
// user "John" created
// John
// deleting John...
// Program ended with exit code: 0
  ```
  
</details>
  
<details>
  <summary>placement ::new with std::malloc</summary>
  
  ```cpp
  #include <string>
#include <iostream>

class User {
public:
    User(std::string&& name) : name_(name)
    {
        std::cout << "user \"" << name << "\" created using std::malloc and ::new" << std::endl;
    }
    
    ~User()
    {
        std::cout << "deleting " << name_ << "..." << std::endl;
    }
    
    void print_name() { std::cout << name_ << std::endl; }
    
private:
    std::string name_;
};

int main()
{
    void *memory = std::malloc(sizeof(User));
    User *u1 = ::new(memory) User("John");
    
    u1->print_name();
    
    u1->~User();
    std::free(memory);
    
    return 0;
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// user "John" created using std::malloc and ::new
// John
// deleting John...
// Program ended with exit code: 0
  ```
  
</details>

With C++17 we can construct and destruct objects _without_ allocating or deallocating memory (really impressive).
  
<details>
  <summary>C++17 method</summary>
  
  ```cpp
  #include <string>
#include <iostream>
#include <memory>

class User {
public:
    User(std::string&& name) : name_(name)
    {
        std::cout << "user \"" << name << "\" created using reinterpret_cast and std::unitialized_fill_at" << std::endl;
    }
    
    ~User()
    {
        std::cout << "deleting " << name_ << "..." << std::endl;
    }
    
    void print_name() { std::cout << name_ << std::endl; }
    
private:
    std::string name_;
};

int main()
{
    void *memory = std::malloc(sizeof(User));
    
    User *user_ptr = reinterpret_cast<User*>(memory);
    
    std::uninitialized_fill_n(user_ptr, 1, User("John"));
    
    user_ptr->print_name();
    
    std::destroy_at(user_ptr);
    std::free(memory);
    
    return 0;
}

  ```
  
</details>
  
And now in C++20, we have `std::construct_at` to replace `std::unitialized_fill_n`
  
  
<details>
  <summary>C++20</summary>
  
  ```cpp
  #include <string>
#include <iostream>
#include <memory>

class User {
public:
    User(std::string&& name) : name_(name)
    {
        std::cout << "user \"" << name << "\" created using reinterpret_case and std::construct_at" << std::endl;
    }
    
    ~User()
    {
        std::cout << "deleting " << name_ << "..." << std::endl;
    }
    
    void print_name() { std::cout << name_ << std::endl; }
    
private:
    std::string name_;
};

int main()
{
    void *memory = std::malloc(sizeof(User));
    
    User *user_ptr = reinterpret_cast<User*>(memory);
    
    std::construct_at(user_ptr, User("John"));
    
    user_ptr->print_name();
    
    std::destroy_at(user_ptr);
    std::free(memory);
    
    return 0;
}

  ```
</details>

#
### `new` and `delete` operators
I've already done a full breakdown of these operators in C++ Primer all the way up to C++17 [here](https://github.com/ITHelpDec/CPP-Primer/blob/f0b1d8cba07f2b00accf0937696cb11cd8a85898/Chapter%2019%20–%20Specialised%20Tools%20and%20Techniques/19.01.cpp).
#
### Alignment
With portability in mind, we can check alignment by using `std::align` over modulo (`%`), or also `std::max_align_t` (which we will use to write custom memory allocations later on).

C++20 joins the party with the `std::has_single_bit` function from the `<bit>` header to check that the argument passed isn't `nullptr` and that alignment is a power of 2 (stated as a requirement in the C++ standard).
<details>
  <summary>Checking alignment</summary>
  
  ```cpp
#include <iostream>
#include <cassert>
#include <bit>

bool is_aligned(void* ptr, std::size_t alignment) {
    assert(ptr != nullptr);
    assert(std::has_single_bit(alignment));

    std::size_t s = std::numeric_limits<std::size_t>::max();
    void *aligned_ptr = ptr;
    std::align(alignment, 1, aligned_ptr, s);

    return ptr == aligned_ptr;
}

int main()
{
    char *p = new char;

    char *p1 = new char('a');
    char *p2 = new char('b');

    std::cout << alignof(std::max_align_t) << std::endl;

    std::size_t max_alighment = alignof(std::max_align_t);

    assert(is_aligned(p, max_alighment));
    assert(is_aligned(p1, max_alighment));
    assert(is_aligned(p2, max_alighment));

    return 0;
}
  ```
  
</details>

<details>
  <summary>Specifying a custom alignment</summary>
  
  ```cpp
  
#include <iostream>

struct alignas(64) Buffer {
    std::byte data[64];
};

int main()
{
    alignas(32) int x;
    alignas(64) int y;

    std::cout << "Buffer: " << alignof(Buffer) << '\n';
    std::cout << "x       " << alignof(x)      << '\n';
    std::cout << "y       " << alignof(y)      << '\n';

    return 0;
}
  ```
</details>

#
### Padding
**This has been the most interesting part of the chapter.**
  
By changing the order of the elements in an object, we can influence the overall size of the object - smaller size means faster iteration, so this is an easy, easy win for performance!
  
<details>
  <summary>Using padding to improve performance</summary>
  
  ```cpp
  #include <iostream>

 class Document1 {
     bool is_cached_;
     double rank_;
     int id_;
 };

 class Document2 {
     bool is_cached_;
     int id_;
     double rank_;
 };

 class Document3 {
     double rank_;
     bool is_cached_;
     int id_;
 };

 class Document4 {
     double rank_;
     int id_;
     bool is_cached_;
 };

 class Document5 {
     int id_;
     double rank_;
     bool is_cached_;
 };

 class Document6 {
     int id_;
     bool is_cached_;
     double rank_;
 };

 int main()
 {
     std::cout << "bool:      " << sizeof(bool)      << " bytes\n";     // 1
     std::cout << "double:    " << sizeof(double)    << " bytes\n";     // 8
     std::cout << "int:       " << sizeof(int)       << " bytes\n\n";   // 4

     std::cout << "Document1: " << sizeof(Document1) << " bytes\n";     // 24
     std::cout << "Document2: " << sizeof(Document2) << " bytes\n";     // 24
     std::cout << "Document3: " << sizeof(Document3) << " bytes\n";     // 24
     std::cout << "Document4: " << sizeof(Document4) << " bytes\n";     // 24
     std::cout << "Document5: " << sizeof(Document5) << " bytes\n";     // 24
     std::cout << "Document6: " << sizeof(Document6) << " bytes\n\n";   // 24

     return 0;
 }
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
// bool:      1 bytes
// double:    8 bytes
// int:       4 bytes
// 
// Document1: 24 bytes
// Document2: 16 bytes
// Document3: 16 bytes
// Document4: 16 bytes
// Document5: 24 bytes
// Document6: 16 bytes
// 
// Program ended with exit code
  ```
  
</details>
  
> _"...it should also be mentioned that it can be beneficial to place multiple data members that are frequently used together"_ – pg. 210
#
### Ownership
**_RAII_** is a concept of huge importance in the pursuit of memory-safe code through predictable object lifetimes.

<details>
<summary>RAIIConnection class</summary>

```cpp
class RAIIConnection {
public:
    explicit RAIIConnection(const std::string& url) : connection_(open_connection(url)) { }
    
    ~RAIIConnection()
    {
        try {
            close(connection_);
        } catch (const std::exception &e) {
            // Handle error, but never throw from a destructor
        }
    }
    
    Connection& get() { return connection_; }
private:
    Connection connection_;
};

void send_request(const std::string& request) {
    RAIIConnection connection("https://eyebleach.me/kittens/");
    send_request(connection.get(), request);

    // close(connection);
    // No need to close the connection above
    // it is automatically handled by the RAIIConnection destructor
}

```
</details>

#
### Containers
> _"It's also possible to use `std::optional` to handle the lifetime of an object that might or might not exist. `std::optional` can be seen as a container with a maximum size of 1."_ – pg. 213

I'm not 100% sure on this statement, as `sizeof(std::optional<T>)` returns a value double the size of what `T` is normally... 
#
### Smart pointers
These are really nice conceptual explanations of smart ponters.

> * _"`std::unique_ptr` – unique ownership expresses tat I, and only I, own the object. When I'm done using it, I will delete it."_ 
> * _"`std::shared_ptr` – shared ownership expresses that I own the object along with others. WHen no one needs the object anymore, it will be deleted."_
> * _"`std::weak_ptr` – weak ownership expresses that I'll use the object if it exists, but don't keep it alive for me. "_

#
### `std::unique_ptr`
The safest and least complicated.

> _"Unique ownership can be transferred to someone else, but it cannot be coppied..."_
```cpp
auto owner = std::make_unique<User>("John");
auto new_owner = std::move(owner);
```
Unique pointers are very efficient, but cannot be passed in a CPU register when being used as part of a function – this makes them _slower_ than raw pointers.
#
### `std::shared_ptr`
We use reference cournting to keep track of the number of owners and object has – at 0 owners, the obejct is deleted.

`std::shared_ptr` is internally thread-safe, so the counter needs to be updated atomically to prevent race conditions.

`std::make_shared<T>` is the preferred method, performing half of the work for the same outcome (reminds me of `++it` vs `it++`);
<details>
<summary>std::make_shared&lt;T&gt;</summary>

```cpp
#include <iostream>

void* operator new (std::size_t size) {
    void *p = std::malloc(size);
    std::cout << "allocated " << size << " byte(s)" << '\n';
    return p;
}

void operator delete(void *p) noexcept {
    std::cout << "deleted memory\n";
    return std::free(p);
}

int main()
{
    auto i = std::make_shared<double>(42.0);
    
    return 0;
}

// allocated 32 byte(s)
// deleted memory
// Program ended with exit code: 0
```
</details>

<details>
<summary>std::shared_ptr&lt;T&gt;(new T())</summary>

```cpp
#include <iostream>

void* operator new (std::size_t size) {
    void *p = std::malloc(size);
    std::cout << "allocated " << size << " byte(s)" << '\n';
    return p;
}

void operator delete(void *p) noexcept {
    std::cout << "deleted memory\n";
    return std::free(p);
}

int main()
{
    auto j = std::shared_ptr<double>(new double(42.0));
    
    return 0;
}

// allocated 8 byte(s)
// allocated 32 byte(s)
// deleted memory
// deleted memory
// Program ended with exit code: 0
```
</details>

#
### `std::weak_ptr`
This is a really nice description of the reasons for using a `std::weak_ptr`
> _"Weak ownership doesn't keepany objects alive; it only allows us to use an object if someone else owns it."_ – pg. 216
  
> _"One common reason for using a weak pointer is to break a reference cycle. A reference cycle occurs when two or more objects refer to each other using shared pointers. Even if all external std::shared_ptr constructors are gone, the objects are kept alive by referring to themselves."_ – pg. 216
  
> _"A weak pointer is safe to use since we cannot reference the object unless it actually exists, which is not the case with a dangling raw pointer."_ – pg. 216

<details>
<summary>std::weak_ptr&lt;T&gt;</summary>

```cpp
#include <iostream>

int main()
{
    std::shared_ptr<int> i = std::make_shared<int>(10);
    
    std::weak_ptr<int> wptr(i);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // .lock() convered std::weak_ptr to std::shared_ptr
    if (auto shared_i = wptr.lock()) {
        std::cout << *shared_i << '\n';
    } else {
        std::cout << "wptr has expired; shared_ptr was nullptr\n";
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    i.reset();
    
    // dereferencing a reset shared_ptr will throw an exception
    if (i) std::cout << *i << '\n';
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    if (auto shared_i = wptr.lock()) {
        std::cout << *shared_i << '\n';
    } else {
        std::cout << "wptr has expired; shared_ptr was nullptr\n";
    }
    
    return 0;
}
```
</details>
  
#
### Small object optimisation
Below is an interesting example of how std::vector dynamically allocates memory when the contents extend beyond the normal realms of the stack size.
<details>
<summary>std::vector</summary>

```cpp
#include <iostream>

std::size_t allocated = 0;

void* operator new(std::size_t size) {
    void *p = std::malloc(size);
    allocated += size;
    return p;
}

void operator delete(void *p) noexcept {
    std::free(p);
}

int main()
{
    // default size
    std::string s;
    
    std::cout << "stack space = " << sizeof(s)    << ','
              << " heap space = " << allocated    << ','
              << " capacity = "   << s.capacity() << '\n';
    
    // 22 characters + '\0'
    std::string t = "1234567890123456789012";
    
    std::cout << "stack space = " << sizeof(t)    << ','
              << " heap space = " << allocated    << ','
              << " capacity = "   << t.capacity() << '\n';
    
    // 23 characters + '\0' – goes beyond stack
    std::string u = "12345678901234567890123";
    
    std::cout << "stack space = " << sizeof(u)    << ','
              << " heap space = " << allocated    << ','
              << " capacity = "   << u.capacity() << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// stack space = 24, heap space = 0, capacity = 22
// stack space = 24, heap space = 0, capacity = 22
// stack space = 24, heap space = 32, capacity = 31
// Program ended with exit code: 0
```
</details>

<details>
<summary>Usings unions</summary>

```cpp
#include <iostream>

// heap version
struct Long {
    std::size_t capacity_;
    std::size_t size_;
    char *data_;
};

// stack version
struct Short {
    unsigned char size_;
    char data_[23];
};

union u_ {
    // flag used to opt between the two e.g. an enum
    // enum { SHORT, LONG };
    Short short_;
    Long long_;
};

int main()
{
    std::cout << "Long: " << sizeof(Long)  << '\n';
    std::cout << "Short " << sizeof(Short) << '\n';
    std::cout << "u_    " << sizeof(u_)    << '\n';
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Long: 24
// Short 24
// u_    24
// Program ended with exit code: 0
```
</details>

#
### Custom memory management
`new` and `std::malloc` are very powerful, but there may be instances where we can eek extra performance by creating our own custom memory allocator.

We can do this by:
1. Analysing the exact memory usage patterns, and
2. Implementing an arena

#
### Building an arena
Knowing the following conditions in advance can help us optimise our area:
1. Single-threaded (no need for atomics)
2. Fixed-size allocations (easier to reclaim memory without worrying about fragmentation)
3. Limited lifetime (we can time reclamation more efficiently, although need to know size ahead of time)

[Arena.h](Arena/Arena.h) | [main.cpp](Arena/main.cpp)

Xcode does a great job of highlighting potential memory leaks, either through **_Xcode Analyse_** or through **_Instruments_**

<details>
<summary>Xcode Analyse in action</summary>

![](Xcode%20Analyse.png)
</details>

#
### Custom memory allocation
Our previous `Arena.h` header does not support STL containers or unique pointers – we can add support to these containers by creating a custom memory allocation.

Below is an simplified example of _The Mallocator_, as well as a simplified implementation of Howard Hinnant's `short_alloc` and a test `main.cpp`

[mallocator.h](mallocator.h) | [short_alloc.h](short_alloc/short_alloc.h) | [main.cpp](short_alloc/main.cpp)
