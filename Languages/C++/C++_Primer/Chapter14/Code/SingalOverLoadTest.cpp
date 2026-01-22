#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;


class Base;

ostream& operator<< (ostream& os, const Base& b);

class Base
{
    friend ostream& operator<< (ostream& os, const Base& b);
public:
    Base(int data, string str = "") : m_baseData(data), m_baseStr(new string(str)) {}
    ~Base() { delete m_baseStr; }

    Base(const Base& other) : m_baseData(other.m_baseData), m_baseStr(new string(*(other.m_baseStr))) {}
    Base& operator=(const Base& other)
    {
        if (this != &other)
        {
            m_baseData = other.m_baseData;
            m_baseStr = new string(*(other.m_baseStr));
        }
        return *this;
    }

    // 下标运算符重载
    char& operator[](size_t index)
    {
        if (index >= m_baseStr->size())
        {
            throw out_of_range("Index out of range");
        }
        return (*m_baseStr)[index];
    }

    const char& operator[](size_t index) const
    {
        if (index >= m_baseStr->size())
        {
            throw out_of_range("Index out of range");
        }
        return (*m_baseStr)[index];
    }

    // 递增运算符重载
    Base& operator++() // 前置递增
    {
        ++m_baseData;
        return *this;
    }

    Base operator++(int) // 后置递增
    {
        Base temp = *this;
        ++m_baseData;
        return temp;
    }

    // 递减运算符重载
    Base& operator--() // 前置递减
    {
        --m_baseData;
        return *this;
    }

    Base operator--(int) // 后置递减
    {
        Base temp = *this;
        --m_baseData;
        return temp;
    }

    // 简单成员函数
    int getData() const { return m_baseData; }
    string getStr() const { return *m_baseStr; }
    void setData(int data) { m_baseData = data; }
    void setStr(const string& str) { *m_baseStr = str; }
private:
    int m_baseData;
    string* m_baseStr;
};

ostream& operator<< (ostream& os, const Base& b)
{
    os << "Base(data: " << b.m_baseData << ", str: " << *b.m_baseStr << ")";
    return os;
}

class BasePtr
{
public:
    BasePtr(Base* base) : m_basePtr(make_unique<Base>(base)) {}
    ~BasePtr() { 
        m_basePtr.reset();
        m_basePtr = nullptr;
    }

    bool check() const
    {
        if (!m_basePtr)
        {
            throw runtime_error("Unbound BasePtr");
        }
        return true;
    }

    // 重载解引用运算符
    Base& operator*() const
    {
        if (check())
        {
            return *m_basePtr;
        }
        throw runtime_error("Unbound BasePtr");
    }

    // 重载成员访问运算符
    Base* operator->() const
    {
        return &this->operator*();
    }

private:
    unique_ptr<Base>  m_basePtr;
};


class BaseFunc
{
public:
    BaseFunc(int data) : m_baseData(data) {}
    ~BaseFunc() {}

    // 函数调用运算符重载
    void operator()(int& value) const
    {
        value += m_baseData;
        return;
    }
private:
    int m_baseData;
};


void signalOverLoadTest(void)
{
    Base b(10, "hello");

    // 通过下标运算符访问字符串中的字符
    cout << b[0] << endl; // 访问第一个字符

    // 通过递增运算符修改数据成员
    Base b1 = ++b; // 前置递增
    cout << b1 << endl;
    Base b2 = b++; // 后置递增
    cout << b2 << endl;

    // 解引用和成员访问运算符
    BasePtr bp(new Base(20, "world"));
    cout << (*bp).getData() << endl; // 使用解引用运算符
    cout << bp->getStr() << endl; // 使用成员访问运算符

    // 函数调用运算符
    vector<int> vewtor = {1, 2, 3, 4, 5};
    for_each(vewtor.begin(), vewtor.end(), BaseFunc(5)); // 使用函数调用运算符   为每个元素加5
    for (const auto& val : vewtor)
    {
        cout << val << " "; // 输出修改后的元素
    }
    cout << endl;

    for_each(vewtor.begin(), vewtor.end(), [](int& value) { value += 10; }); // 使用lambda表达式   为每个元素加10
    for (const auto& val : vewtor)
    {
        cout << val << " "; // 输出修改后的元素
    }
    cout << endl;
    return;    
}

void standardAlgorithmFunc(void)
{
    // 使用标准库中的函数对象
    
    // 算数
    plus<int> intAdd; // 使用标准库中的函数对象
    cout << intAdd(3, 4) << endl; // 输出7

    // 关系
    vector<int> vec = {1, 2, 3, 4, 5};
    int count = count_if(vec.begin(), vec.end(), bind2nd(less<int>(), 3)); // 统计小于3的元素个数
    cout << count << endl; // 输出2

    // 逻辑
    vector<bool> boolVec = {true, false, true, true, false};
    int trueCount = count_if(boolVec.begin(), boolVec.end(), logical_not<bool>()); // 统计false的个数
    cout << trueCount << endl; // 输出2

    // 位
    int a = 5; // 0101
    int b = 3; // 0011
    bit_and<int> bitAnd; // 使用标准库中的位运算函数对象
    cout << bitAnd(a, b) << endl; // 输出1 (0001)

    // 其他
    negate<int> intNegate; // 使用标准库中的取反函数对象
    cout << intNegate(a) << endl; // 输出-5

    // 适配器
    vector<int> vec2 = {1, 2, 3, 4, 5};
    transform(vec2.begin(), vec2.end(), vec2.begin(), bind1st(multiplies<int>(), 2)); // 每个元素乘以2
    for (const auto& val : vec2)
    {
        cout << val << " "; // 输出2 4 6 8 10
    }
    cout << endl;

    return;
}

int customAdd(int a, int b)
{
    return a + b;
}

void standardFunctionTest(void)
{
    // 普通函数
    function<int(int, int)> func = customAdd; // 使用标准库中的function封装函数指针
    cout << func(3, 4) << endl; // 输出7

    // lambda表达式
    function<int(int, int)> lambdaFunc = [](int a, int b) { return a + b; }; // 使用lambda表达式
    cout << lambdaFunc(5, 6) << endl; // 输出11

    // 成员函数
    Base b(10, "hello");
    function<int(const Base&)> memFunc = &Base::getData; // 使用成员函数指针
    cout << memFunc(b) << endl; // 输出10

    return;
}

class SmallInt; // 前向声明
SmallInt operator+(const SmallInt& lhs, const SmallInt& rhs); // 声明加法运算符

class SmallInt
{
    friend SmallInt operator+(const SmallInt& lhs, const SmallInt& rhs);
public:
    SmallInt(int value) : m_value(value) 
    {
        if (value < 0 || value > 255)
        {
            throw out_of_range("Value must be between 0 and 255");
        }
    }

    SmallInt& operator=(int value)
    {
        if (value < 0 || value > 255)
        {
            throw out_of_range("Value must be between 0 and 255");
        }
        m_value = value;
        return *this;
    }

    operator int() const
    {
        return m_value;
    }

private:
    int m_value;
};

SmallInt operator+(const SmallInt& lhs, const SmallInt& rhs)
{
    int sum = lhs.m_value + rhs.m_value;
    if (sum > 255)
    {
        throw out_of_range("Sum exceeds maximum value of 255");
    }
    return SmallInt(sum);
}

void smallIntTest(void)
{
    SmallInt si(100);
    // int value = si; // 隐式转换为int
    // cout << value << endl; // 输出100

    si = 200; // 使用赋值运算符
    cout << static_cast<int>(si) << endl; // 输出200

    try
    {
        si = 300; // 超出范围，抛出异常
    }
    catch (const out_of_range& e)
    {
        cout << e.what() << endl; // 输出异常信息
    }

    SmallInt si1(100);
    SmallInt si2(150);

    SmallInt si3 = si1 + si2; // 使用加法运算符
    cout << static_cast<int>(si3) << endl; // 输出250

    int x = si3 + 5; // 二义性调用  // SmallInt + int 还是 int + SmallInt

    return;
}

class B;
class A
{
public:
    A() = default;
    A(const B&) {}
};

class B
{
public:
    B() = default;
    operator A() const { }
};

void ambiguousTest(void)
{
    A f(const A&);
    A a1;
    B b1;
    A a2 = f(b1); // 二义性调用
    return;
}


int main(void)
{
    // signalOverLoadTest();

    // standardAlgorithmFunc();

    // standardFunctionTest();

    smallIntTest();

    return 0;
}