#include <thread/promise.h>
#include <iostream>


int main(int argc, char **argv)
{

    Easy::Promise<int>([]{
        std::cout << 1 << std::endl;
        return 1;
    }).then<std::string>([](const Easy::ResultBlock<int>& closure) {
        std::cout << 2 << std::endl;
       return std::to_string(closure());
    }).then<std::string>([](const Easy::ResultBlock<std::string>& closure) {
        auto s = closure();
        std::cout << 3 << std::endl;
        return s + "123456";
    }).then<int>([](const Easy::ResultBlock<std::string>& closure) {

        auto s = closure();
        std::cout << 4 << std::endl;
        return 0;
    }).when([](const std::exception& e) {
        std::cout << e.what() << std::endl;
    }).wait();


    return 0;
}
