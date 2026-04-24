#include <iostream>
#include <vector>

#include "Lockable.h"
#include "Waitable.h"

using namespace threadx;

int main() {
    Lockable<std::string> str = " ";
    Lockable<std::string> str1 = "1";
    Lockable<std::string> str2 = str;
    Lockable<std::string> str3 = std::move(str);
    str2 = str1;
    str3 = std::move(str1);

    std::cout << std::boolalpha;
    std::cout << *str2.lock() << ' ' << *str3.lock() << std::endl;
    std::cout << str.try_lock().has_value() << ' ' << str.try_lock().has_value() << std::endl;

    Waitable<std::vector<int>> v {1};
    Waitable<std::vector<int>> v1 {2, 3};
    Waitable<std::vector<int>> v2 = v;
    Waitable<std::vector<int>> v3 = std::move(v);
    v2 = v1;
    v3 = std::move(v1);

    std::cout << v2.lock()->size() << ' ' << v3.lock()->size() << std::endl;
    std::cout << v.try_lock().has_value() << ' ' << v.try_lock().has_value() << std::endl;
}