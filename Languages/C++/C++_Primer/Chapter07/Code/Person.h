#include <string>
#include <iostream>

class Person {
    
public:
    Person() = default;

    Person(const std::string &name, const std::string &address)
        : name(name), address(address) {}

    // 练习7.12 
    Person(std::istream &is)
    {
        read(is);
    }

    void display() const 
    {
        std::cout << "Name: " << name << ", Address: " << address << std::endl;
    }

    // 练习7.9 添加read和print成员函数
    std::istream &read(std::istream &is) 
    {
        std::cout << "Enter name: ";
        std::cout << "Enter address: ";
        return is >> name >> address;
    }

    std::ostream &print(std::ostream &os) const
    {
        return os << "Name: " << name << ", Address: " << address << std::endl;
    }

private:
    std::string name;
    std::string address;
};
