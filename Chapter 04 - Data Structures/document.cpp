#include <string>
#include <memory>

class Document {
public:
    Document(std::string &&title) : title_(title) { }
private:
    std::string title_;
};

struct Hit {
    float rank_;
    std::shared_ptr<Document> document_;
};

#include <vector>
#include <queue>
#include <iostream>

bool cmp(const Hit &lhs, const Hit &rhs)
{
    return lhs.rank_ > rhs.rank_;
}

template <typename ForwardIt>
std::vector<Hit> sort_hits(ForwardIt begin, ForwardIt end, std::size_t m) {
    std::vector<Hit> result(m);

    std::priority_queue<Hit, std::vector<Hit>, decltype(cmp)*> pq;

    for (auto it = begin; it != end; ++it) {
        if (pq.size() < m) { pq.push(*it); }
        else if (it->rank_ > pq.top().rank_) {
            pq.pop();
            pq.push(*it);
        }
    }

    // this block feels quite expensive...have written it differently below
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    while (!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }

    std::reverse(result.begin(), result.end());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    return result;
}

void printVec(const std::vector<int> &ivec) {
    for (const auto &e : ivec) {
        std::cout << e << " ";
    } std::cout << std::endl;
}

int main()
{
    std::priority_queue<int> pq;
    pq.push(6);
    pq.push(4);
    pq.push(8);
    pq.push(3);
    pq.push(9);

    std::size_t m = 5;


    // without the need to call std::reverse
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    std::vector<int> result(m);

    while (m--) {
        result[m] = pq.top();
        pq.pop();
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    printVec(result);

    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 3 4 6 8 9
