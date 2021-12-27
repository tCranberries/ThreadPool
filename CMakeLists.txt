cmake_minimum_required(VERSION 3.21)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(ThreadPool
        example.cpp
        ThreadPool.h)

target_link_libraries(ThreadPool
        pthread)


add_executable(_ThreadPool
        main.cpp _ThreadPool.h)

target_link_libraries(_ThreadPool
        pthread)

add_executable(test test.cpp test.h)

target_link_libraries(test
        pthread)

add_executable(ThreadSafeStack ThreadSafeStack.cpp ThreadSafeStack.h)

target_link_libraries(ThreadSafeStack
        pthread)