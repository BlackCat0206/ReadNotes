#include <memory>
#include <iostream>

using namespace std;

void sharedPtrTest(void)
{
    shared_ptr<int> p1(new int(42));
    shared_ptr<int> p2 = make_shared<int>(43);
    auto p3 = p1.get();

    // delete p3; // Undefined behavior: p1 now points to a deleted object

    cout << "*p1: " << *p1 << endl;
    cout << "*p2: " << *p2 << endl;
    cout << "p1.use_count(): " << p1.use_count() << endl;
    cout << "p2.use_count(): " << p2.use_count() << endl;

    return;

}

int main(void)
{
    sharedPtrTest();

    return 0;
}