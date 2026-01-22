#include <iostream>
#include <string>
#include <utility>

using namespace std;

void rvalueReferenceDemo()
{
       int i = 10;
    int &r = i; // r is an lvalue reference to i

    int &&r1 = 20; // r1 is an rvalue reference to a temporary integer with value 20
    int &&r2 = i + 10; // r2 is an rvalue reference to the result of i + 10, which is a temporary integer with value 20

    cout << "i = " << i << endl;       // Outputs: i = 10
    cout << "r1 = " << r1 << endl;     // Outputs: r1 = 20
    cout << "r2 = " << r2 << endl;     // Outputs: r2 = 20

    int && r3 = std::move(i); // r3 is an rvalue reference to i, which is treated as an rvalue
    r3 += 10; // Modifies i through r3
    cout << "After modifying r3, i = " << i << endl; // Outputs

    int && r4 = std::move(r1); // r4 is an rvalue reference to r1, which is treated as an rvalue
    r4 += 10; // Modifies r1 through r4
    cout << "After modifying r4, r1 = " << r1 << endl;

    int && r5 = std::move(r); // r5 is an rvalue reference to r, which is treated as an rvalue
    r5 += 10; // Modifies i through r5
    cout << "After modifying r5, r = " << r << endl;

    int& r6 = r1; // r6 is an lvalue reference to r1
    r6 += 10; // Modifies r1 through r6
    cout << "After modifying r6, r1 = " << r1 << endl;
    
    return;
}

class Base;
void swap(Base& lhs, Base& rhs);

class Base 
{
    friend void swap(Base& lhs, Base& rhs);
public:
    Base(int y = 0, string s = string("")):x(y), str(new string(s)) { cout << "Base Constructor" << endl; }
    
    Base(const Base& rhs);
    Base(Base&& rhs) noexcept;
    
    Base& operator=(const Base& rhs);
    Base& operator=(Base&& rhs) noexcept;

    // Example of member function that can only be called on lvalue objects
    void add(int val) &;
    void add(int val) &&; // Overloaded for rvalue objects

    ~Base() {
        delete str;
        str = nullptr; 
        cout << "Base Destructor" << endl; 
    }
private:
    int x;
    string* str = nullptr;
};

void swap(Base& lhs, Base& rhs) 
{
    using std::swap; // Enable ADL
    cout << "addr: lhs.x = " << &lhs.x << ", addr: rhs.x = " << &rhs.x << endl;
    swap(lhs.x, rhs.x);
    cout << "After swap: addr: lhs.x = " << &lhs.x << ", addr: rhs.x = " << &rhs.x << endl;
    
    cout << "addr: lhs.str = " << lhs.str << ", addr: rhs.str = " << rhs.str << endl;
    swap(lhs.str, rhs.str);
    cout << "After swap: addr: lhs.str = " << lhs.str << ", addr: rhs.str = " << rhs.str << endl;
}

//  Copy constructor
Base::Base(const Base& rhs):x(rhs.x), str(new string(*(rhs.str))) 
{ 
    cout << "Base Copy Constructor" << endl; 
}

// Move constructor
Base::Base(Base&& rhs) noexcept : x(rhs.x), str(rhs.str) {
    rhs.str = nullptr; // Leave rhs in a valid state
    cout << "Base Move Constructor" << endl;
}

// Copy assignment operator
Base& Base::operator=(const Base& rhs) 
{
    cout << "Base Copy Assignment operator" << endl;
    if (this != &rhs) {
        x = rhs.x;
        delete str; // Free existing resource
        str = new string(*(rhs.str)); // Allocate new resource and copy
    }
    return *this;
}

// Move assignment operator
Base& Base::operator=(Base&& rhs) noexcept {
    cout << "Base Move Assignment operator" << endl;
    if (this != &rhs) {
        delete str; // Free existing resource
        x = rhs.x;
        str = rhs.str;
        rhs.str = nullptr; // Leave rhs in a valid state
    }
    return *this;
}

void Base::add(int val) & 
{
    x += val;
    cout << "Lvalue add: x = " << x << endl;
}

void Base::add(int val) && 
{
    x += val;
    cout << "Rvalue add: x = " << x << endl;
}

int main(void)
{
    // rvalueReferenceDemo();

    Base b1(10, "Hello");
    Base b2(20, "World");

    b2 = std::move(b1); // Move assignment operator is called here
    b1 = Base(30, "Temporary"); // Move assignment operator is called here

    string s = "Sample";
    string s1 = "Another Sample";
    cout << "s + s1 = " << (s + s1) << endl;

    s + s1 = "Modified"; // Error: lvalue required as left operand of assignment

    cout << "s = " << s << endl; // s remains unchanged

    auto n = (s + s1).find("ple");
    cout << "n = " << n << endl;

    return 0;
}
