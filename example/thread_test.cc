#include <thread/thread.h>
#include <iostream>

int main(int argc, char **argv)
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

  return 0;
}
