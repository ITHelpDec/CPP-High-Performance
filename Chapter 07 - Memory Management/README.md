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
#
### ...next
