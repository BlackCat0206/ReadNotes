#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

using std::string;
using std::cin;
using std::cout;
using std::vector;

enum Color 
{
    red, 
    blue, 
    green
};

void add_string(void)
{
    string s1 = "Hello, ";
    string s2 = "World!";
    string s3 = s1 + s2;

    cout << s3;
    return;
}

vector<int>::difference_type find_sum(vector<int>&v, const int &traget)
{
    auto begin = v.begin();
    auto end = v.end();
    auto mid = begin + (end - begin) / 2;
    while (begin < end && traget != (*mid))
    {
        if (traget < *mid)
        {
            end = mid;
        }
        else
        {
            begin = mid + 1;
        }
        mid = begin + (end - begin) / 2;
    }

    if (traget == *mid)
    {
        cout << "find it" << std::endl;
        return (mid - v.begin());
    }
    else
    {
        cout << "not find it" << std::endl;
        return -1;
    }
}

void test_try_throw(void)
{
    // get two numbers from standard input
    int v1 = 0, v2 = 0;
    std::cin >> v1 >> v2;
    if (v2 == 0)
    {
        throw std::runtime_error("divisor is 0");
    }

    cout << v1 / v2 << std::endl;
    return;
}

void test_try_throw2(void)
{
    // get two numbers from standard input
    int v1 = 0, v2 = 0;
    while(cin >> v1 >> v2)
    {
        try
        {
            if (v2 == 0)
            {
                throw std::runtime_error("divisor is 0");
            }
        }
        catch (std::runtime_error err)
        {
            cout << err.what() << std::endl;
            cout << "Try again? (y/n)" << std::endl;
            char c;
            cin >> c;
            if (!cin || c == 'n')
            {
                break;
            }
            continue;
        }
        cout << v1 / v2 << std::endl;
    }
    return;
}

void switch_color(Color c)
{
    switch (c)
    {
        case red:
            cout << "red" << std::endl;
            break;
        case blue:
            cout << "blue" << std::endl;
            break;
        case green:
            cout << "green" << std::endl;
            break;
        default:
            break;
    }
    return;
}


void refTest(int &i)
{
    i = 10;
    return;
}


const char* getname(void)
{
    return "hello";
}

const string& getStr(void)
{
    return "hello world";
}

int main(void)
{
    add_string();

    vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
    int traget = 5;
    
    auto res = find_sum(v, traget);
    cout << "the traget index is " << res << std::endl;

    // test_try_throw();
    // test_try_throw2();

    int i = 0;
    int &r = i;
    refTest(r);
    cout << "i = " << i << std::endl;

    const char* name = getname();
    cout << name << std::endl;

    const string &s = getStr();
    cout << s << std::endl;

    return 0;
}