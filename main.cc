#include <iostream>
#include <string>

#include "expression.h"

int main()
{
    std::string exp;
    if (!std::getline(std::cin, exp))
    {
        return 1;
    }

    std::string result = expression::expand(exp);
    if (result.empty())
    {
        return 1;
    }

    std::cout << expression::expand(exp) << std::endl;
    return 0;
}
