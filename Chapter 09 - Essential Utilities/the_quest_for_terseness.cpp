#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

struct Player {
    const char *name_;
    int level_ = 0;
    int score_ = 0;
};

void printPlayerVec(const std::vector<Player> &pvec) {
    for (const auto &[name, level, score] : pvec) {
        std::cout << name << ": " << level << ", " << score << '\n';
    } std::cout << '\n';
}

int main()
{
    std::vector<Player> players_template = {
        { "Doug", 69, 9001 },
        { "Bert", 9, 240 },
        { "Alfred", 69, 314  },
    };
    
    std::cout << "players_template:\n"; printPlayerVec(players_template);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::vector<Player> players1(players_template);
    
    std::sort(players1.begin(), players1.end(), [] (const Player &lhs, const Player &rhs) {
        return (lhs.level_ == rhs.level_) ? lhs.score_ < rhs.score_
                                          : lhs.level_ < rhs.level_;
    } );
    
    std::cout << "players1:\n"; printPlayerVec(players1);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::vector<Player> players2(players_template);
    
    std::sort(players2.begin(), players2.end(), [] (const Player &lhs, const Player &rhs) {
        return std::tie(lhs.level_, lhs.score_) < std::tie(rhs.level_, rhs.score_);
    } );
    
    std::cout << "players2:\n"; printPlayerVec(players2);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    std::vector<Player> players3(players_template);
    
    // quite a nice use of std::ranges
    std::ranges::sort(players3, std::less(), [] (const Player &p) {
        return std::tie(p.level_, p.score_);
    } );
    
    std::cout << "players3:\n"; printPlayerVec(players3);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// players_template:
// Doug: 69, 9001
// Bert: 9, 240
// Alfred: 69, 314

// players1:
// Bert: 9, 240
// Alfred: 69, 314
// Doug: 69, 9001

// players2:
// Bert: 9, 240
// Alfred: 69, 314
// Doug: 69, 9001
  
// players3:
// Bert: 9, 240
// Alfred: 69, 314
// Doug: 69, 9001

// Program ended with exit code: 0
