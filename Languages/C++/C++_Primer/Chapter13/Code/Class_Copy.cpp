#include <iostream>
#include <string>

using namespace std;

class Base;
void swap(Base& lhs, Base& rhs);

class Base 
{
    friend void swap(Base& lhs, Base& rhs);
public:
    Base(int y = 0, string s = string("")):x(y), str(new string(s)) { cout << "Base Constructor" << endl; }
    Base(const Base& rhs):x(rhs.x), str(new string(*(rhs.str))) { cout << "Base Copy Constructor" << endl; }
    // Base& operator=(const Base& rhs);
    Base& operator=(Base rhs); // Pass by value to leverage copy constructor
    ~Base() {
        delete str;
        str = nullptr; 
        cout << "Base Destructor" << endl; 
    }
private:
    int x;
    string* str = nullptr;
};

// Base& Base::operator=(const Base& rhs) 
// {
//     cout << "Base Assignment operator" << endl;
//     if (this != &rhs) {
//         x = rhs.x;
//         delete str; // Free existing resource
//         str = new string(*(rhs.str)); // Allocate new resource and copy
//     }
//     return *this;
// }

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

Base& Base::operator=(Base rhs) // Pass by value to leverage copy constructor
{
    cout << "Base Assignment operator (copy-and-swap)" << endl;
    swap(*this, rhs); // Swap the contents
    return *this;
}

int main() 
{
    cout << "\nExiting main" << endl;

    Base b1(10);
    Base b2 = b1; // Copy constructor is called here
    
    Base b3;
    b3 = b1;      // Assignment operator is called here
    
    return 0;
}