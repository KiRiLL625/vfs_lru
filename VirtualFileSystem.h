#ifndef L3_VIRTUALFILESYSTEM_H
#define L3_VIRTUALFILESYSTEM_H

#include <memory>
#include <iostream>
#include <functional>
#include <filesystem>
#include "Set.h"

class VirtualFileSystem {
private:
    struct Node { // Структура узла
        std::string name; // Имя
        std::string realPath; // Реальный путь (для директорий пустая строка)
        bool isDirectory; // Флаг директории
        Set<std::shared_ptr<Node>> children;  // Используем Set для хранения детей (для быстрого поиска), дети - указатели на узлы
        //shared_ptr используется для того, чтобы не копировать узлы, а хранить указатели на них

        Node(std::string name, bool isDirectory, std::string realPath = "") // Конструктор узла
            : name(std::move(name)), realPath(std::move(realPath)), isDirectory(isDirectory) {}
    };

    std::shared_ptr<Node> root; // Корень

    std::shared_ptr<Node> findNode(const std::string &path) { // Поиск узла по пути
        if (path == "/") { // Если путь - корень
            return root;
        }

        std::vector<std::string> parts = splitPath(path); // Разбиваем путь на части
        auto current = root; // Текущий узел - корень
        for (const auto &part : parts) { // Проходим по частям пути
            auto it = std::find_if(current->children.begin(), current->children.end(), [&part](const std::shared_ptr<Node> &child) { // Ищем узел с нужным именем
                return child->name == part; // Сравниваем имена
            }); // Итератор на найденный узел

            if (it == current->children.end() || !(*it)->isDirectory) { // Если узел не найден или это не директория
                return nullptr; // Возвращаем nullptr
            }

            current = *it; // Переходим к следующему узлу
        }
        return current; // Возвращаем найденный узел
    }

    std::vector<std::string> splitPath(const std::string &path) { // Разбиение пути на части
        std::vector<std::string> parts; // Вектор частей пути
        size_t start = 0, end = 0; // Начальная и конечная позиции
        while ((end = path.find('/', start)) != std::string::npos) { // Пока находим слэш
            if (end != start) { // Если путь не пустой
                parts.push_back(path.substr(start, end - start)); // Добавляем часть пути
            }
            start = end + 1; // Переходим к следующей части
        }
        if (start < path.size()) { // Если осталась последняя часть
            parts.push_back(path.substr(start)); // Просто добавляем её
        }
        return parts; // Возвращаем вектор частей
    }

    void printTree(const std::shared_ptr<Node> &node, const std::string &prefix, bool isLast) { // Вывод дерева
        std::cout << prefix << (isLast ? "└── " : "├── ") << node->name; // Выводим имя узла

        if (!node->isDirectory) { // Если это файл
            std::cout << " -> " << node->realPath; // Выводим реальный путь
        }
        std::cout << std::endl;

        for (auto it = node->children.begin(); it != node->children.end(); ++it) { // Проходим по детям (узлам)
            printTree(*it, prefix + (isLast ? "    " : "│   "), std::next(it) == node->children.end()); // Рекурсивно выводим дерево
        }
    }

public:
    VirtualFileSystem() : root(std::make_shared<Node>("/", true)) {} // Конструктор по умолчанию (корень - истинная директория)

    void addFile(const std::string &virtualPath, const std::string &realPath, const std::string &fileName) { // Добавление файла
        if (!std::filesystem::exists(realPath)) { // Если реальный путь не существует
            throw std::runtime_error("Real path does not exist: " + realPath);
        }
        auto parent = findNode(virtualPath); // Ищем узел по виртуальному пути
        if (!parent || !parent->isDirectory) { // Если узел не найден или это не директория
            throw std::runtime_error("Invalid path: " + virtualPath);
        }

        auto fileNode = std::make_shared<Node>(fileName, false, realPath); // Создаём узел файла
        parent->children.insert(fileNode); // Вставляем файл с помощью Set
    }

    void addDirectory(const std::string &virtualPath, const std::string &dirName) { // Добавление директории
        auto parent = findNode(virtualPath); // Ищем узел по виртуальному пути
        if (!parent || !parent->isDirectory) { // Если узел не найден или это не директория
            throw std::runtime_error("Invalid path: " + virtualPath);
        }

        auto dirNode = std::make_shared<Node>(dirName, true); // Создаём узел директории
        parent->children.insert(dirNode);  // Вставляем директорию с помощью Set
    }

    void removeFile(const std::string &virtualPath, const std::string &fileName) { // Удаление файла
        auto parent = findNode(virtualPath); // Ищем родительскую директорию
        if (!parent || !parent->isDirectory) { // Если родитель не найден или не директория
            throw std::runtime_error("Invalid path: " + virtualPath);
        }

        // Ищем файл в дочерних элементах
        auto it = std::find_if(parent->children.begin(), parent->children.end(),
                               [&fileName](const std::shared_ptr<Node> &child) {
                                   return child->name == fileName && !child->isDirectory; // Найдем файл
                               });

        if (it != parent->children.end()) { // Если файл найден
            parent->children.remove(*it); // Удаляем файл с помощью метода remove
            std::cout << "File " << fileName << " removed." << std::endl;
        } else {
            throw std::runtime_error("File not found: " + fileName);
        }
    }

    void removeDirectory(const std::string &virtualPath, const std::string &dirName) { // Удаление директории
        auto parent = findNode(virtualPath); // Ищем родительскую директорию
        if (!parent || !parent->isDirectory) { // Если родитель не найден или не директория
            throw std::runtime_error("Invalid path: " + virtualPath);
        }

        // Ищем директорию среди дочерних элементов
        auto it = std::find_if(parent->children.begin(), parent->children.end(),
                               [&dirName](const std::shared_ptr<Node> &child) {
                                   return child->name == dirName && child->isDirectory; // Найдем директорию
                               });

        if (it != parent->children.end()) { // Если директория найдена
            if ((*it)->children.size() == 0) { // Проверяем, что директория пуста
                parent->children.remove(*it); // Удаляем директорию
                std::cout << "Directory " << dirName << " removed." << std::endl;
            } else {
                throw std::runtime_error("Directory is not empty: " + dirName);
            }
        } else {
            throw std::runtime_error("Directory not found: " + dirName);
        }
    }


    void printStructure() { // Вывод структуры
        std::cout << root->name << std::endl;
        for (auto it = root->children.begin(); it != root->children.end(); ++it) { // Проходим по детям корня
            printTree(*it, "", std::next(it) == root->children.end()); // Вызываем функцию вывода дерева с нужными параметрами
        }
    }
};

//Так много shared_ptr из-за того, что Set хранит shared_ptr на узлы, чтобы не копировать их, а хранить указатели

#endif //L3_VIRTUALFILESYSTEM_H
