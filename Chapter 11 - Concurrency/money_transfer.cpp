#include <mutex>
#include <iostream>

struct Account {
    Account(int &&amount) : balance_(amount){ }
    std::mutex m_;
    int balance_ = 0;
};

void transferMoney(Account &from, Account &to, int amount) {
    std::unique_lock<std::mutex> lock1 = { from.m_, std::defer_lock };
    std::unique_lock<std::mutex> lock2 = {   to.m_, std::defer_lock };
    
    std::lock(lock1, lock2);
    
    from.balance_ -= amount;
      to.balance_ += amount;
}

int main()
{
    Account acc1 = 100, acc2 = 200;
    std::cout << "acc1: " << acc1.balance_ << ", acc2: " << acc2.balance_ << '\n';
    
    transferMoney(acc2, acc1, 50);
    
    std::cout << "acc1: " << acc1.balance_ << ", acc2: " << acc2.balance_ << '\n';
    
    return 0;
}
