// bad hash
#include <string>
#include <unordered_set>
#include <iostream>

struct Person {
    Person(std::string&& name, std::size_t &&age) : name_(std::move(name)), age_(std::move(age)) { }
    
    std::string name_;
    std::size_t age_;
};

auto bad_hash_lambda = [] (const Person& person) {
    return 47;
};

auto person_eq_lambda = [] (const Person &lhs, const Person &rhs) {
    return lhs.name_ == rhs.name_ && lhs.age_ == rhs.age_;
};

// templated print function
template <typename T> void printSet(const T &t) {
    for (const auto &p : t) {
        std::cout << p.name_ << ": " << p.age_ << "\n";
    } std::cout << std::endl;
}

int main()
{
    Person p1("man", 42), p2("bear", 314), p3("pig", 69);
    
    std::unordered_set<Person, decltype(bad_hash_lambda), decltype(person_eq_lambda)>
        pset_lambda = { p1, p2, p3 };
    
    printSet(pset_lambda);
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// pig: 69
// bear: 314
// man: 42

// Program ended with exit code: 0
