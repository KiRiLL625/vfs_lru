cmake_minimum_required(VERSION 3.27.4)
project(l3)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(testing)
add_subdirectory(googletest)

add_executable(l3 main.cpp BST.h Set.h
        ISet.h
        VirtualFileSystem.h
        IDictionary.h
        Dictionary.h
        LRUCache.h
        ICache.h
)
