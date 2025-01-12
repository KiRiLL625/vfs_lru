cmake_minimum_required(VERSION 3.25)
project(test)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_executable(test test.cpp ../BST.h ../Set.h
        ../ISet.h
        ../VirtualFileSystem.h
        ../IDictionary.h
        ../Dictionary.h
        ../LRUCache.h
        ../ICache.h
)
target_link_libraries(test gtest gtest_main)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include(GoogleTest)