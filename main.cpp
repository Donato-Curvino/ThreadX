#include <iostream>

#include "Lockable.h"

using namespace threadx;

int main() {
    Lockable<std::string> str = " ";
    Lockable<std::string> str1 = "1";
    Lockable<std::string> str2 = str;
    Lockable<std::string> str3 = std::move(str);
    str2 = str1;
    str3 = std::move(str1);

    std::cout << *str2.lock() << ' ' << *str3.lock() << std::endl;
}