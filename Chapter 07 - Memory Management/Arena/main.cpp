#include "Arena.h"

Arena<1024> user_arena;

class ArenaUser {
public:
    void* operator new(std::size_t size) { return user_arena.allocate(size); }
    
    void operator delete(void *p)
    {
        return user_arena.deallocate(static_cast<std::byte*>(p), sizeof(ArenaUser));
    }
    
    void* operator new[](std::size_t size) { return user_arena.allocate(size); }
    
    void operator delete[](void *p)
    {
        return user_arena.deallocate(static_cast<std::byte*>(p), sizeof(ArenaUser));
    }
    
private:
    int id_;
};

class User {
private:
    int id_;
};

std::size_t allocated = 0;

void* operator new(std::size_t size) {
    void *p = std::malloc(size);
    allocated += size;
    return p;
}

void operator delete(void *p) noexcept {
    std::free(p);
    allocated -= sizeof(p);
}

void* operator new[](std::size_t size) {
    void *p = std::malloc(size);
    allocated += size;
    return p;
}

void operator delete[](void *p) noexcept {
    std::free(p);
    allocated -= sizeof(p);
}

void memstuff()
{
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ArenaUser - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    ArenaUser *arenauser1 = new ArenaUser;
    std::cout << "ArenaUser: " << sizeof(arenauser1) << " bytes (stack), "
              << allocated     << " bytes (heap)\n";
    delete arenauser1;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    ArenaUser *arenausers = new ArenaUser[10];
    std::cout << "ArenaUser: " << sizeof(arenausers) << " bytes (stack), "
              << allocated     << " bytes (heap)\n";
    delete [] arenausers;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    auto arenauser2 = std::make_unique<ArenaUser>();
    std::cout << "ArenaUser: " << sizeof(arenauser2) << " bytes (stack), "
              << allocated     << " bytes (heap)\n\n";
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // User  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    User *user1 = new User;
    std::cout << "User: "  << sizeof(user1) << " bytes (stack), "
              << allocated << " bytes (heap)\n";
    delete user1;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    User *users = new User[10];
    std::cout << "User: "  << sizeof(users) << " bytes (stack), "
              << allocated << " bytes (heap)\n";
    delete [] users;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    auto user2 = std::make_unique<User>();
    std::cout << "User: "  << sizeof(user2) << " bytes (stack), "
              << allocated << " bytes (heap)\n\n";
}

int main()
{
    memstuff();
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// ArenaUser: 8 bytes (stack), 0 bytes (heap)
// ArenaUser: 8 bytes (stack), 0 bytes (heap)
// ArenaUser: 8 bytes (stack), 0 bytes (heap)
//
// User: 8 bytes (stack), 4 bytes (heap)
// User: 8 bytes (stack), 36 bytes (heap)
// User: 8 bytes (stack), 32 bytes (heap)
//
// Program ended with exit code: 0
