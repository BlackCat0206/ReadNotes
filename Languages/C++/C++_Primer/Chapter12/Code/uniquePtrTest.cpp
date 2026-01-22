#include <memory>
#include <iostream>

using namespace std;

void uniquePtrTest(void)
{
    unique_ptr<int> p1(new int(42));
    unique_ptr<int> p2 = make_unique<int>(43);
    
    p2.reset(p1.release()); // Transfer ownership from p1 to p2

    unique_ptr<int> p3(p2.release()); // Transfer ownership from p2 to p3

    // cout << "*p2: " << *p2 << endl; // Undefined behavior: p2 is now nullptr
    cout << "*p3: " << *p3 << endl;

    return;

}


unique_ptr<int> createUniquePtr(void)
{
    unique_ptr<int> p(new int(100));
    return p; // Return by value, ownership is transferred to the caller
}

int main(void)
{

    uniquePtrTest();

    unique_ptr<int> p4 = createUniquePtr();

    int * p5 = new int(200);
    unique_ptr<int> p6(p5); // Take ownership of p5

    unique_ptr<int[]> p7(new int[5]{1, 2, 3, 4, 5}); // Unique pointer to an array

    for (size_t i = 0; i < 5; ++i)
    {
        cout << "p7[" << i << "]: " << p7[i] << endl;
    }

    shared_ptr<int> p8 = make_shared<int>(300); // weak_ptr can be created from shared_ptr


    cout << "*p4: " << *p4 << endl;

    cout << "*p6: " << *p6 << endl;

    return 0;
}