cmake_minimum_required(VERSION 2.8)

project(Thread)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(THREAD_SRC
        src/thread.cc
        src/thread_queue.cc
        src/promise.cc
    )

add_library(thread STATIC ${THREAD_SRC})
target_link_libraries(thread pthread rt)



add_executable(thread_test example/thread_test.cc)
target_link_libraries(thread_test thread)
