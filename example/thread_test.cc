#include <thread/thread.h>
#include <thread/promise.h>
#include <iostream>


void threadQueuetTest();
void promiseTest();
int main(int argc, char **argv)
{

//  threadQueueTest();
  promiseTest();
  return 0;
}


void threadQueueTest()
{
  auto queue = Easy::Threading::getQueue(Easy::Threading::Serial); //获取串行队列
  //  auto queue = Easy::Threading::getQueue(Easy::Threading::Concurrent); //获取队列
  //  auto queue = Easy::Threading::getQueue(Easy::Threading::Serial); //获取串行队列

  queue->dispatch([]{
    std::cout << "第1个任务" << std::endl;
  });
  queue->dispatch([]{
    std::cout << "第2个任务" << std::endl;
  });
  queue->dispatch([]{
    std::cout << "第3个任务" << std::endl;
  });
  queue->dispatch([]{
    std::cout << "第4个任务" << std::endl;
  });

  Easy::Threading::sleep(1);
}

void promiseTest()
{
    Easy::Promise<int>([]{
        std::cout << 1 << std::endl;
        return 1;
    }).then<std::string>([](const int last) {
        std::cout << "2: " << last << std::endl;
       return std::to_string(last);
    }).then<std::string>([](const std::string& last) {
        std::cout << "3: " << last  << std::endl;
        return last + "123456";
    }).then<int>([](const std::string& last) {
        std::cout << "4: "  << last << std::endl;
        return 0;
    }).then<int>([](int last) {
        throw std::runtime_error("test");
        return last + 1;
    }).when([](const std::exception& e) {
        std::cout << e.what() << std::endl;
    });
 
}
