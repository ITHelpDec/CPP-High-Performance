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
### ...next
