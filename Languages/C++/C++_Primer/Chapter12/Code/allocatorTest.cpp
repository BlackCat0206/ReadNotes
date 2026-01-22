#include <memory>
#include <iostream>
#include <vector>

using namespace std;

void allocatorTest(void)
{
    // Allocator usage example
    allocator<int> alloc; // Create an allocator for int
    int * p = alloc.allocate(5); // Allocate space for 5 ints

    for (size_t i = 0; i < 5; ++i)
    {
        alloc.construct(&p[i], static_cast<int>(i) * 10); // Construct ints in allocated space
    }

    for (size_t i = 0; i < 5; ++i)
    {
        cout << "p[" << i << "]: " << p[i] << " ";
    }
    cout << endl;

    for (size_t i = 0; i < 5; ++i)
    {
        alloc.destroy(&p[i]); // Destroy the constructed ints
    }

    alloc.deallocate(p, 5); // Deallocate the space

    return;
}

void allocatorAlgrothmTest(void)
{
    allocator<int> alloc; // Create an allocator for int
    int * p = alloc.allocate(10); // Allocate space for 5 ints

    vector<int> vecNum({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    uninitialized_copy(vecNum.begin(), vecNum.end(), p); // Copy elements to allocated space

    for (size_t i = 0; i < 10; ++i)
    {
        cout << "p1[" << i << "]: " << p[i] << " ";
    }
    cout << endl;

    auto end = uninitialized_fill_n(p, 10, 0); // Fill allocated space with 42

    unsigned size = end - p;

    cout << "size: " << size << endl;

    for (size_t i = 0; i < 10; ++i)
    {
        cout << "p2[" << i << "]: " << p[i] << " ";
    }
    cout << endl;

    while(end != p)
    {
        alloc.destroy(--end); // Destroy the constructed ints
    }

    alloc.deallocate(p, 10); // Deallocate the space

    return;
}

class A
{
public:
    A()=default;
    ~A()=default;
    void Test() const;
    void Test();
};
void A::Test() const
{
    cout << "A::Test() const" << endl;
}

void A::Test()
{
    cout << "A::Test()" << endl;
}

int main(void)
{
    allocatorTest();

    allocatorAlgrothmTest();

    int&& i = 42;
    
    int &y = i;

    y = 43;

    cout << "i: " << i << endl;

    A a;
    a.Test();
    const A ca;
    ca.Test();

    return 0;
}