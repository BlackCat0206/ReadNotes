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
    void print(Base& b) const override;



private:
    int mi_derived;
};