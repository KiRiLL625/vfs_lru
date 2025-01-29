#ifndef L3_VIRTUALFILESYSTEM_H
#define L3_VIRTUALFILESYSTEM_H

#include <iostream>
#include <stdexcept>
#include <string>
#include "ArraySequence.h"
#include "Set.h"  // Подключаем контейнер Set
#include "Dictionary.h"  // Подключаем хэш-таблицу Dictionary

class VirtualFileSystem {
private:
    struct Node {
        std::string name;
        std::string realPath;
        bool isDirectory;
        Dictionary<std::string, Node*> children;  // Используем Dictionary для хранения дочерних узлов

        Node(const std::string& name, bool isDirectory, const std::string& realPath = "")
            : name(name), realPath(realPath), isDirectory(isDirectory) {}

        ~Node() {
            auto keys = children.keys();  // Получаем все ключи из Dictionary
            for (size_t i = 0; i < keys.getLength(); ++i) {
                delete children.get(keys.get(i));  // Удаляем дочерние узлы по ключу
            }
        }

    };

    Node* root;
    Set<std::string> uniquePaths;  // Контейнер для хранения уникальных виртуальных путей

    Node* findNode(const std::string& path) {
        if (path == "/") {
            return root;
        }

        ArraySequence<std::string> parts = splitPath(path);
        Node* current = root;

        for (int i = 0; i < parts.getLength(); ++i) {
            const std::string& part = parts[i];
            if (!current->children.contains_key(part)) {
                return nullptr;  // Узел не найден
            }
            current = current->children.get(part);
        }

        return current;
    }

    ArraySequence<std::string> splitPath(const std::string& path) {
        ArraySequence<std::string> parts;
        size_t start = 0, end = 0;

        while ((end = path.find('/', start)) != std::string::npos) {
            if (end != start) {
                parts.append(path.substr(start, end - start));
            }
            start = end + 1;
        }

        if (start < path.size()) {
            parts.append(path.substr(start));
        }

        return parts;
    }

    void printTree(Node* node, const std::string& prefix, bool isLast) {
        if (!node) return;  // Проверяем, что узел существует

        std::cout << prefix << (isLast ? "└── " : "├── ") << node->name;

        if (!node->isDirectory) {
            std::cout << " -> " << node->realPath;
        }
        std::cout << std::endl;

        auto keys = node->children.keys();  // Получаем ключи из Dictionary
        for (size_t i = 0; i < keys.getLength(); ++i) {
            bool lastChild = (i == keys.getLength() - 1);
            printTree(node->children.get(keys.get(i)), prefix + (isLast ? "    " : "│   "), lastChild);
        }
    }


public:
    VirtualFileSystem() {
        root = new Node("/", true);
        uniquePaths.insert("/");  // Корневой путь
    }

    ~VirtualFileSystem() {
        //delete root;
    }

    void addFile(const std::string& virtualPath, const std::string& realPath, const std::string& fileName) {
        std::string fullPath = virtualPath + "/" + fileName;
        if (uniquePaths.contains(fullPath)) {
            throw std::runtime_error("Path already exists: " + fullPath);
        }

        Node* parent = findNode(virtualPath);
        if (!parent || !parent->isDirectory) {
            throw std::runtime_error("Invalid path: " + virtualPath);
        }

        Node* fileNode = new Node(fileName, false, realPath);
        parent->children.add(fileName, fileNode);
        uniquePaths.insert(fullPath);
    }

    void addDirectory(const std::string& virtualPath, const std::string& dirName) {
        std::string fullPath = virtualPath + "/" + dirName;
        if (uniquePaths.contains(fullPath)) {
            throw std::runtime_error("Path already exists: " + fullPath);
        }

        Node* parent = findNode(virtualPath);
        if (!parent || !parent->isDirectory) {
            throw std::runtime_error("Invalid path: " + virtualPath);
        }

        Node* dirNode = new Node(dirName, true);
        parent->children.add(dirName, dirNode);
        uniquePaths.insert(fullPath);
    }

    void removeFile(const std::string& virtualPath, const std::string& fileName) {
        Node* parent = findNode(virtualPath);
        if (!parent || !parent->isDirectory) {
            throw std::runtime_error("Invalid path: " + virtualPath);
        }

        if (!parent->children.contains_key(fileName) || parent->children.get(fileName)->isDirectory) {
            throw std::runtime_error("File not found: " + fileName);
        }

        delete parent->children.get(fileName);
        parent->children.remove(fileName);
        uniquePaths.remove(virtualPath + "/" + fileName);
    }

    void removeDirectory(const std::string& virtualPath, const std::string& dirName) {
        Node* parent = findNode(virtualPath);
        if (!parent || !parent->isDirectory) {
            throw std::runtime_error("Invalid path: " + virtualPath);
        }

        if (!parent->children.contains_key(dirName) || !parent->children.get(dirName)->isDirectory) {
            throw std::runtime_error("Directory not found: " + dirName);
        }

        if (parent->children.get(dirName)->children.count() != 0) {
            throw std::runtime_error("Directory is not empty: " + dirName);
        }

        delete parent->children.get(dirName);
        parent->children.remove(dirName);
        uniquePaths.remove(virtualPath + "/" + dirName);
    }

    void printStructure() {
        if (!root) {
            std::cerr << "Tree is empty." << std::endl;
            return;
        }
        std::cout << root->name << std::endl;

        auto keys = root->children.keys();  // Получаем ключи из корневого Dictionary
        for (size_t i = 0; i < keys.getLength(); ++i) {
            bool lastChild = (i == keys.getLength() - 1);
            printTree(root->children.get(keys.get(i)), "", lastChild);
        }
    }

};

#endif // L3_VIRTUALFILESYSTEM_H
