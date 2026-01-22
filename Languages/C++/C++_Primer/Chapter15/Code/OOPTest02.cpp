#include <iostream>

using namespace std;

class A
{
public:
    A(int x) : val(x) {}
    virtual void show() const {
        cout << "Value: " << val << endl;
    }
private:
    int val;
};


class B : public A
{
public:
    B(int x, int y) : A(x), val(y) {}
    void show() const
    {
        cout << "B Value" << val << endl;
    }
private:
    int val;
};


class C : public B
{
public:
    C(int x, int y, int z) : B(x, y), val(z) {}
    void show() const
    {
        cout << "C Value" << val << endl;
    }
private:
    int val;
};

class D : public C
{
public:
    D(int x, int y, int z) : C(x, y, z) {}
    void show() const
    {
        C::show();
    }
};

int main(void)
{
    C obj(1, 2, 3);

    A* pa = &obj;
    B* pb = &obj;

    pa->show();
    pb->show();

    D obj2(1, 2, 3);
    A* pa2 = &obj2;
    B* pb2 = &obj2;

    pa2->show();
    pb2->show();



    return 0;
}