#include <iostream>
#include <type_traits>

struct Bear {
    void roar() const { std::cout << "roar!\n";}
};

struct Dog {
    void bark() const { std::cout << "woof!\n"; }
};

// _"the compiler will keep the call to the member function, quack(), ...
// ...hich will then fail to compile since Bear does not contain a quack() member function"_
template <typename Animal> void speak(const Animal &a) {
    // if (std::is_same_v<Animal, Bear>) {
    if constexpr (std::is_same_v<Animal, Bear>) {
        a.roar();
    // } else if (std::is_same_v<Animal, Dog>){
    } else if constexpr (std::is_same_v<Animal, Dog>){
        a.bark();
    // } else { // finds our issues at compile-time
    //     static_assert(false, "not an animal\n");;
    }
}

int main()
{
    Bear bear;
    speak(bear);
    
    Dog dog;
    speak(dog);
    
    // int x = 69;
    // speak(x);
          
    return 0;
}
