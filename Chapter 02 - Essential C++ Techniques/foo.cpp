struct Foo {
    // performs like "const"
    void func() && { std::cout << "This object is an rvalue" << std::endl; }
    
    // for normal lvalue refs
    void func() & { std::cout << "This object is an lvalue ref" << std::endl; }
};

int main() {
    Foo bar;
    
    // without separate & and && member function modifiers we receive the following error:
    // 'this' argument to member function 'func' is an lvalue, but function has rvalue ref-qualifier
    
    // lvalue ref
    bar.func();
    
    // rvalue
    std::move(bar).func();
    
    // rvalue
    Foo().func();
}
