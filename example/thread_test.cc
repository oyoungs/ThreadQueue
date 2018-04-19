#include <thread/thread.h>
#include <iostream>


int main(int argc, char **argv)
{

    auto q = Easy::Threading::getQueue("queue", Easy::Threading::Concurrent);

    q->dispatch([]{
        Easy::Threading::sleep(1.0);
        std::cout << "dispatch1" << std::endl;
    });
    q->dispatch([]{
        Easy::Threading::sleep(1.0);
        std::cout << "dispatch2" << std::endl;
    });
    q->dispatch([]{
        Easy::Threading::sleep(1.0);
        std::cout << "dispatch3" << std::endl;
    });
    q->dispatch([]{
        Easy::Threading::sleep(1.0);
        std::cout << "dispatch4" << std::endl;
    });
    q->dispatch([]{
        Easy::Threading::sleep(1.0);
        std::cout << "dispatch5" << std::endl;
    });
    std::cout << "CPU: " << Easy::Threading::processorCount() << std::endl;
    Easy::Threading::sleep(10.0);


    return 0;
}
