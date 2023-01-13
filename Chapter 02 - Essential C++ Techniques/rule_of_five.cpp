#include <initializer_list>
#include <iostream>

class Buffer {
public:
    // (1) default constructor
    Buffer(const std::initializer_list<float> &values) : size_(values.size())
    {
        ptr_ = new float[values.size()];
        std::copy(values.begin(), values.end(), ptr_);
        
        std::cout << "Buffer()" << std::endl;
    }
    
    // (2) copy constructor
    Buffer(const Buffer&);
    
    // (3) copy-assignment operator
    Buffer& operator=(const Buffer&);
    
    // (5) move constructor
    Buffer(Buffer&&) noexcept;
    
    // (6) move-assignment operator
    Buffer& operator=(Buffer&&) noexcept;
    
    // (4) - destructor
    ~Buffer();
    
    auto begin() { return ptr_; }
    auto end() { return ptr_ + size_; }
private:
    std::size_t size_ = 0;
    float *ptr_ = nullptr;
};

// (2) - copy constructor
Buffer::Buffer(const Buffer &rhs) : size_(rhs.size_)
{
    ptr_ = new float[size_];
    std::copy(rhs.ptr_, (rhs.ptr_ + size_), ptr_);
    
    std::cout << "Buffer(const Buffer&)" << std::endl;
}

// (3) - copy-assignment operator
Buffer& Buffer::operator=(const Buffer &rhs)
{
    // need to protect against self-assignment!!
    // copy the underlying buffer from rhs in case lhs points to rhs
    // would prefer to encapsulate temp_p in a smart pointer, on the off chance...
    // ... that an exception is thrown during allocation and the allocation...
    // ... is not destroyed
    std::size_t temp_sz = rhs.size_;
    auto temp_p = new float[temp_sz];
    std::copy(rhs.ptr_, (rhs.ptr_ + rhs.size_), temp_p);
    
    delete [] ptr_;
    
    ptr_ = new float[temp_sz];
    std::copy(temp_p, (temp_p + temp_sz), ptr_);
    size_ = temp_sz;
    
    delete [] temp_p;
    temp_p = nullptr;
    
    std::cout << "operator=(const Buffer&)" << std::endl;
    
    return *this;
}

// (4) - destructor
Buffer::~Buffer()
{
    delete [] ptr_;
    ptr_ = nullptr;
    
    std::cout << "~Buffer()" << std::endl;
}

// (5) - move constructor
// use std::move() on members of class type
// use std::exchange() on members of non-class type
Buffer::Buffer(Buffer &&rhs) noexcept : size_(std::exchange(rhs.size_, 0)), ptr_(std::exchange(rhs.ptr_, nullptr))
{
    std::cout << "Buffer(Buffer&&) noexcept" << std::endl;
}

// (6) move-assignment operator
Buffer& Buffer::operator=(Buffer &&rhs) noexcept
{
    if (this != &rhs) {
        ptr_ = std::exchange(rhs.ptr_, nullptr);
        size_ = std::exchange(rhs.size_, 0);
    }
    std::cout << "Buffer& operator=(Buffer&&) noexcept" << std::endl;
    return *this;
}

int main()
{
    Buffer b0 = { 0.0f, 0.5f, 1.0f, 1.5f };
    std::cout << "b0: ";
    for (const auto &e : b0) { std::cout << e << " "; } std::cout << "\n" << std::endl;
    
    Buffer b1 = b0;
    std::cout << "b1: ";
    for (const auto &e : b1) { std::cout << e << " "; } std::cout << "\n" << std::endl;
    
    b0 = b1;
    std::cout << "b0: ";
    for (const auto &e : b0) { std::cout << e << " "; } std::cout << "\n" << std::endl;
    
    b1 = std::move(b0);
    std::cout << "b1: ";
    for (const auto &e : b1) { std::cout << e << " "; } std::cout << std::endl;
    std::cout << "b0: ";
    for (const auto &e : b0) { std::cout << e << " "; } std::cout << "\n" << std::endl;
  
    return 0;
}
