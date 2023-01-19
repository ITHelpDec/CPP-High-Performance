#include <string>
#include <unordered_set>
#include <iostream>

struct Person {
    Person(std::string&& name, std::size_t &&age) : name_(std::move(name)), age_(std::move(age)) { }

    std::string name_;
    std::size_t age_;
};

namespace boost {
template<typename T>
inline void hash_combine(std::size_t &seed, const T &v) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
}

// boost hash_combine (definition above)
auto person_hash_lambda = [](const Person& person) {
    std::size_t seed = 0;
    boost::hash_combine(seed, person.name_);
    boost::hash_combine(seed, person.age_);
    return seed;
};

auto person_eq_lambda = [] (const Person &lhs, const Person &rhs) {
    return lhs.name_ == rhs.name_ && lhs.age_ == rhs.age_;
};

template <typename T> void printSet(const T &t) {
    for (const auto &p : t) {
        std::cout << p.name_ << ": " << p.age_ << "\n";
    } std::cout << std::endl;
}

int main()
{
    Person p1("man", 42), p2("bear", 314), p3("pig", 69);

    std::unordered_set<Person, decltype(person_hash_lambda), decltype(person_eq_lambda)>
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
