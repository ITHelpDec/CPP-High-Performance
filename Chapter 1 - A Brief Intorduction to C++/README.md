# C++ High Performance (2nd Edition)

## Highlights from Chapter 1 - "A Brief Introduction to C++"

### The importance of zero-cost abstrations within C++.
**"Bjarne Stroustrup, the inventor of C++, defines the zero-overheadprinciple like this: "** – pg. 4
> _"What you don't use, you don't pay for."_

>_"What you do use, you couldn't hand-code any better."_
#
### Improved terse idiom moving from C to C++ with little change to the machine code

linked_list.c | linked_list.cpp
#
### The intricacies and differences of C++ compared to the likes of Java
* Commpilation (JIT vs direct compilation)
* Memory management (garbage collection vs manually or reference-counting methods)
* Memory allocation (individual heap allocations vs finer control over individual or bulk stack / heap allocation for greater contiguity)
#
### Value Semantics
>_"The C++ type system gives us the ability to explicitly state the ownership of an object" – pg. 9_

>_"In the C++ version, the programmer states that the toppings are completely encapsulated by the Bagel class" – pg. 10_
#
### Const Correctness
>_"Const correctness means that each member function signature of a class explicitly tells the caller whether the object will be modified or not; and it will not compile if the caller tries to modify an object declared const" – pg. 11_
#
### Object Ownership
>_"Except in very rare situations, a C++ programmer should leave the memory handling to containers and smart pointers, and never have to rely on manual memory handling." – pg. 12_
#
### Deterministic Destruction in C++
>_"The destruction of objects is deterministic in C++. That means that we (can) know exactly when an object is being destroyed" – pg. 13_

>_"Deterministic destruction is also one of the features that makes C++ predictable; something that is highly valued among programmers, and a requirement for performance-critical applications." – pg. 13_
#
### Avoiding null objects using C++ references
>_"C++ arguments passed as references indicate that null values are **not** allowed:" – pg. 14_

> _"C++ arguments passed as pointers indicate that null values **are** being handled:" – pg. 14_
#
### Drawbacks of C++
* Initial complexity of the language
* Longer compile times
* Importing of libraries (headers, although now we have modules in C++20)
* Lack of provided libraries (compared to the likes of numpy or itertools in Python – handy, but not a big deal)
