#include <string>
#include <unordered_set>
#include <iostream>

struct Person {
    Person(std::string&& name, std::size_t &&age)
        : name_(std::move(name)), age_(std::move(age)) { }

    std::string name_;
    std::size_t age_;
};

// operator== required for template specialisation
bool operator==(const Person &lhs, const Person &rhs)
{
    return lhs.name_ == rhs.name_ && lhs.age_ == rhs.age_;
}

namespace std {

template <> struct hash<Person> {
    typedef size_t result_type;
    typedef Person argument_type;

    size_t operator()(const Person&) const;
};

size_t hash<Person>::operator()(const Person &p) const {
    return hash<string>()(p.name_) ^ hash<size_t>()(p.age_);
}

}

template <typename T> void printSet(const T &t) {
    for (const auto &p : t) {
        std::cout << p.name_ << ": " << p.age_ << "\n";
    } std::cout << std::endl;
}

int main()
{
    Person p1("man", 42), p2("bear", 314), p3("pig", 69);

    std::unordered_set<Person> pset({ p1, p2, p3 });

    printSet(pset);

    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// pig: 69
// bear: 314
// man: 42

// Program ended with exit code: 0
