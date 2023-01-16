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
        f2();
        f1();
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
        stk_.pop();
    }
    
    void f3()
    {
        stk_.push("f3");
        stkvec_.push_back({ stk_, std::chrono::steady_clock::now() });
        f2();
        stk_.pop();
    }
    
    void print()
    {
        auto start = std::chrono::steady_clock::now();
        for (auto &[stk, tstamp] : stkvec_) {
            while (!stk.empty()) {
                std::cout << stk.top() << std::right << std::setw(8 - stk.top().size()) << "(" << std::chrono::duration_cast<std::chrono::microseconds>(tstamp - start).count() + 90 <<  ")" << std::endl;
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

// f3     (61ms)
// main   (61ms)

// f2     (66ms)
// f3     (66ms)
// main   (66ms)

// f1     (74ms)
// f2     (74ms)
// f3     (74ms)
// main   (74ms)

// f2     (78ms)
// main   (78ms)

// f1     (83ms)
// f2     (83ms)
// main   (83ms)

// f1     (87ms)
// main   (87ms)

// Program ended with exit code: 0
