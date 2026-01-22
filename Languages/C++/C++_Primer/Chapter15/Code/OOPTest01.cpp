#include <iostream>
// #include "Base.h"

using namespace std;


class Base
{
public:
    Base(int i = 0) : mi_base(i) {}
    virtual ~Base() {}

    virtual void print(Base& b) const;

protected:
    int getBase() const { return mi_base; }
private:
    int mi_base;
};


class Derived; // Forward declaration
void getBaseTest(const Derived& d); // Function prototype
void getBaseTest2(const Base& b); // Function prototype

class Derived : public Base
{
    friend void getBaseTest(const Derived& d); // Friend function declaration
    friend void getBaseTest2(const Base& b); // Friend function declaration
public:
    Derived(int i = 0, int j = 0) : Base(i), mi_derived(j) {}
    virtual void print(Base& b) const final override ;

private:
    int mi_derived;
};


void getBaseTest(const Derived& d)
{
    cout << "Derived getBaseTest: " << d.getBase() << endl; // Accessing protected member function
}

void getBaseTest2(const Base& b)
{
    // cout << "Base getBaseTest2: " << b.getBase() << endl; // Accessing protected member function is NOT allowed
}


void Base::print(Base& b) const
{
    cout << "Base: " << mi_base << endl;
}

void Derived::print(Base& b) const
{
    int baseNum = Base::getBase();    // Accessing protected member function
    cout << "Base: " << baseNum << " Derived: " << mi_derived << endl;
}

class Base2
{
public:
    Base2(int i = 0) : mi_base(i) {}
    virtual ~Base2() {}

    virtual void print()const=0;

private:
    int mi_base;
};

void Base2::print() const
{
    cout << "Base2: " << mi_base << endl;
}


class Derived2 : public Base2
{
public:
    Derived2(int i = 0, int x) : Base2(i), mi_derived(x) {}
    void print() const override
    {
        Base2::print();
    }
private:
    int mi_derived;
};

int main()
{
    Base b(1);
    Derived d(2, 3);

    Base* pb = &b;
    Base* pd = &d;

    pb->print(b);
    pd->print(d);

    pd->Base::print(b); // Calling Base class print method

    // pd->getBase(); // Accessing protected member function - Not allowed

    Derived d2(4, 5);
    Base b3(d2); // Upcasting
    b3.print(b3);

    // Base2 b2; // This line will cause a compilation error because Base2 is abstract
    Derived2 d23(6, 7);
    d23.Base2::print();

    return 0;
}