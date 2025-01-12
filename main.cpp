#include <iostream>
#include "VirtualFileSystem.h"
#include "LRUCache.h"

bool menu_vfs(VirtualFileSystem& vfs){
    std::cout << "1. Create directory" << std::endl;
    std::cout << "2. Create file" << std::endl;
    std::cout << "3. Delete directory" << std::endl;
    std::cout << "4. Delete file" << std::endl;
    std::cout << "5. Print file system" << std::endl;
    std::cout << "6. Exit" << std::endl;
    int choice;
    std::cin >> choice;
    switch(choice){
        case 1:{
            std::string path;
            std::cout << "Enter path to directory: ";
            std::cin >> path;
            std::cout << "Enter name of directory: ";
            std::string name;
            std::cin >> name;
            try {
                vfs.addDirectory(path, name);
            }
            catch (std::runtime_error& e) {
                std::cout << "Couldn't find path. Error: " << e.what() << std::endl;
            }
            return true;
        }

        case 2:{
            std::string path;
            std::cout << "Enter path to file: ";
            std::cin >> path;
            std::cout << "Enter name of file: ";
            std::string name;
            std::cin >> name;
            std::cout << "Enter real path to file: ";
            std::string realPath;
            std::cin >> realPath;
            try {
                vfs.addFile(path, realPath, name);
            }
            catch (std::runtime_error& e) {
                std::cout << "Couldn't find path or real path. Error: " << e.what() << std::endl;
            }
            return true;
        }

        case 3:{
            std::string path;
            std::cout << "Enter path to directory: ";
            std::cin >> path;
            std::cout << "Enter name of directory: ";
            std::string name;
            std::cin >> name;
            try {
                vfs.removeDirectory(path, name);
            }
            catch (std::runtime_error& e) {
                std::cout << "Couldn't find path. Error: " << e.what() << std::endl;
            }
            return true;
        }

        case 4:{
            std::string path;
            std::cout << "Enter path to file: ";
            std::cin >> path;
            std::cout << "Enter name of file: ";
            std::string name;
            std::cin >> name;
            try{
                vfs.removeFile(path, name);
            }
            catch (std::runtime_error& e) {
                std::cout << "Couldn't find path. Error: " << e.what() << std::endl;
            }
            return true;
        }

        case 5:{
            vfs.printStructure();
            return true;
        }

        case 6:{
            return false;
        }

        default:{
            std::cout << "Invalid input" << std::endl;
            return true;
        }
    }
}

bool menu_cache(LRUCache<std::string, std::string>& cache){
    std::cout << "1. Add element" << std::endl;
    std::cout << "2. Get element" << std::endl;
    std::cout << "3. Print cache" << std::endl;
    std::cout << "4. Exit" << std::endl;
    int choice;
    std::cin >> choice;
    switch(choice){
        case 1:{
            std::string key;
            std::cout << "Enter key: ";
            std::cin >> key;
            std::string value;
            std::cout << "Enter value: ";
            std::cin >> value;
            cache.access(key, value);
            return true;
        }

        case 2:{
            std::string key;
            std::cout << "Enter key: ";
            std::cin >> key;
            try {
                std::cout << "Value: " << cache.get(key) << std::endl;
                cache.access(key, cache.get(key));
            }
            catch (std::runtime_error& e) {
                std::cout << "Key not found. Error: " << e.what() << std::endl;
            }
            return true;
        }

        case 3:{
            cache.print();
            return true;
        }

        case 4:{
            return false;
        }

        default:{
            std::cout << "Invalid input" << std::endl;
            return true;
        }
    }
}

int main() {
    std::cout << "1. Virtual File System" << std::endl;
    std::cout << "2. LRU Cache" << std::endl;
    int choice;
    std::cin >> choice;
    switch(choice){
        case 1:{
            VirtualFileSystem vfs;
            while(menu_vfs(vfs));
            return 0;
        }

        case 2:{
            LRUCache<std::string, std::string> cache(3);
            while(menu_cache(cache));
            return 0;
        }

        default:{
            std::cout << "Invalid input" << std::endl;
            return 0;
        }
    }
    return 0;
}
