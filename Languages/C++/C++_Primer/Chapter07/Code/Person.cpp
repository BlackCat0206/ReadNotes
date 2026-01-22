#include "Person.h"
#include <vector>
#include <iostream>
#include <functional>


using std::vector;
using std::placeholders::_1;
using std::placeholders::_2;


int main(void)
{
    Person person("Alice", "123 Main St");
    person.display();
    person.read(std::cin);
    person.print(std::cout);

    Person person2(std::cin);
    person2.display();

    

    return 0;
}