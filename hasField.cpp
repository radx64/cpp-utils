#include <iostream>
#include <string>
#include "hasField.hpp"

struct X {int Coffee;};
struct Y {};

int main()
{
    std::cout << HasCoffe<X>::value << std::endl;
    std::cout << HasCoffe<Y>::value << std::endl;
    return 0;
}