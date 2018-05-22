# ThreadQueue
## 一个基于pthread封装的调度队列库，包括了串行队列，并行队列
## 另外提供了一个类似于javascript中promise工具功能类似的Promise类

## 使用Threading示例代码

```cpp
#include <thread/thread.h>
#include <iostream>

int main(int argc, char **argv)
{
  auto queue = Easy::Threading::getQueue(Easy::Threading::Serial); //获取匿名串行队列
  //  auto queue = Easy::Threading::getQueue(Easy::Threading::Concurrent); //获取匿名并行队列
  //  auto queue = Easy::Threading::getQueue("queue", Easy::Threading::Serial); //获取具名串行队列
  //  auto queue = Easy::Threading::getQueue("queue", Easy::Threading::Serial); //获取具名并行队列

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

```

## 使用Promise示例代码

```cpp
#include <thread/promise.h>
#include <iostream>


int main(int argc, char **argv)
{

    Easy::Promise<int>([]{
        std::cout << 1 << std::endl;
        return 1;
    }).then<std::string>([](int last) {
        std::cout << 2 << std::endl;
       return std::to_string(last);
    }).then<std::string>([](const std::string& last) {
        auto s = last;
        std::cout << 3 << std::endl;
        return s + "123456";
    }).then<int>([](const std::string& last) {

        auto s = last;
        std::cout << 4 << std::endl;
        return 0;
    }).when([](const std::exception& e) {
        std::cout << e.what() << std::endl;
    });


    return 0;
}

```
