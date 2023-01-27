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
###
