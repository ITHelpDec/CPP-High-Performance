struct AnimalBase {
    virtual ~AnimalBase() { }
    virtual void speak() const { }
};

struct Bear : public AnimalBase {
    virtual void speak() const override { roar(); }
    void roar() const { std::cout << "rarrrr!\n"; }
};

struct Dog : public AnimalBase {
    virtual void speak() const override { woof(); }
    void woof() const { std::cout << "woof woof!\n"; }
};

void speak(const AnimalBase &a) {
    a.speak();
}

int main()
{
    Bear bear;
    speak(bear);
    
    Dog dog;
    speak(dog);
    
    return 0;
}
