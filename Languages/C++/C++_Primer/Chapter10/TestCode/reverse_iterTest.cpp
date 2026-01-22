#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

int main() 
{
    vector<int> vec = {1, 2, 3, 4, 5};

    // Using reverse iterator to traverse the vector in reverse order
    cout << "Vector elements in reverse order: ";
    for (auto rit = vec.rbegin(); rit != vec.rend(); ++rit) {
        cout << *rit << " ";
    }
    cout << endl;

    auto rcomma = find(vec.rbegin(), vec.rend(), 3);

    cout << *rcomma.base() << endl;

    return 0;
}