#include <vector>
#include <string>
#include <ranges>
#include <iostream>
#include <algorithm>

struct Student {
    int year_;
    int score_;
    std::string name_;
};

std::vector<Student> students = {
    {3, 120, "Niki"},
    {2, 140, "Karo"},
    {3, 190, "Sirius"},
    {2, 110, "Rani"},
};

// quite verbose
int get_max_score(const std::vector<Student> &s, int year) {
    auto by_year = [=] (const Student &s) { return s.year_ == year; };
    
    auto v1 = std::ranges::ref_view(s);
    auto v2 = std::ranges::filter_view(v1, by_year);
    auto v3 = std::ranges::transform_view(v2, &Student::score_);
    
    auto it = std::ranges::max_element(v3);
    
    return it != v3.end() ? *it : 0;
}

int get_max_score_refactor(const std::vector<Student> &s, int year) {
    auto by_year = [=] (const Student &s) { return s.year_ == year; };
    
    // auto v1 - v3 flattened
    using namespace std::ranges;
    auto scores = transform_view(filter_view(ref_view(s), by_year), &Student::score_);
    
    auto it = max_element(scores);
    return it != scores.end() ? *it : 0;
}

int main()
{
    std::cout << "get_max_score:          " << get_max_score(students, 3)          << "\n";
    std::cout << "get_max_score_refactor: " << get_max_score_refactor(students, 3) << "\n";
    
    return 0;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// get_max_score:          190
// get_max_score_refactor: 190
