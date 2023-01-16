// not very polish, but an example of what a sampling profiler could be, as and when functions enter and leave the stack

#include <stack>
#include <string>
#include <chrono>
#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>

class SamplingConcept {
public:
    SamplingConcept()
    {
        stk_.push("main");
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        f3();
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        f2();
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        f1();
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        stk_.pop();
        print();
    }
    
private:
    std::stack<std::string> stk_;
    std::vector<std::pair<std::stack<std::string>, std::chrono::steady_clock::time_point>> stkvec_;
    
    void f1()
    {
        stk_.push("f1");
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        stk_.pop();
    }
    
    void f2()
    {
        stk_.push("f2");
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        f1();
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        stk_.pop();
    }
    
    void f3()
    {
        stk_.push("f3");
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        f2();
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        stk_.pop();
    }
    
    void print()
    {
        auto start = std::chrono::steady_clock::now();
        for (auto &[stk, tstamp] : stkvec_) {
            while (!stk.empty()) {
                std::cout << stk.top() << std::right << std::setw(8 - stk.top().size()) << "("
                          // 63 is just a random number to get the output to equate to 0 - I'll have to rework it properly
                          << std::chrono::duration_cast<std::chrono::microseconds>(tstamp - start).count() + 63 <<  "ms)"
                          << std::endl;
                stk.pop();
            } std::cout << std::endl;
        }
    }
};

int main()
{
    SamplingConcept();
    
    return 0;
}

// output:
// main   (0ms)

// f3     (12ms)
// main   (12ms)

// f2     (17ms)
// f3     (17ms)
// main   (17ms)

// f1     (25ms)
// f2     (25ms)
// f3     (25ms)
// main   (25ms)

// f2     (29ms)
// f3     (29ms)
// main   (29ms)

// f3     (34ms)
// main   (34ms)

// main   (37ms)

// f2     (41ms)
// main   (41ms)

// f1     (44ms)
// f2     (44ms)
// main   (44ms)

// f2     (50ms)
// main   (50ms)

// main   (53ms)

// f1     (57ms)
// main   (57ms)

// main   (60ms)

// Program ended with exit code: 0
