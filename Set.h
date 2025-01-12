//
// Created by Admin on 29.05.2024.
//

#ifndef BST2_SET_H
#define BST2_SET_H

#include "ISet.h"
#include "BST.h"
#include <fstream>

template <typename T>
class Set: public ISet<T> { // Множество на основе AVL-дерева
private:
    AVLTree<T> tree; // Дерево
public:
    Set() {
        tree = AVLTree<T>(); // Создаем пустое дерево
    }

    Set(const Set<T>& other) {
        tree = other.tree; // Копируем дерево
    }

    Set<T>& operator=(const Set<T>& other) {
        tree = other.tree; // Перемещаем дерево
        return *this;
    }

    void insert(T key) override { // Вставка элемента
        if (!tree.contains(key)) {
            tree.insert(key);
        }
    }

    void remove(T key) override { // Удаление элемента
        tree.remove(key);
    }

    bool contains(T key) override { // Проверка наличия элемента
        return tree.contains(key);
    }

    /*
    ISet<T> operator+(ISet<T>& other) override { // Union
        Set<T> result;
        for (T key : this->tree) {
            result.insert(key);
        }
        for (T key : other.tree) {
            result.insert(key);
        }
        return result;
    }

    ISet<T> operator*(ISet<T>& other) override { // Intersection
        Set<T> result;
        for (T key : this->tree) {
            if (other.contains(key)) {
                result.insert(key);
            }
        }
        return result;
    }

    ISet<T> operator-(ISet<T>& other) override { // Difference
        Set<T> result;
        for (T key : this->tree) {
            if (!other.contains(key)) {
                result.insert(key);
            }
        }
        return result;
    }
     */

    typename AVLTree<T>::Iterator begin() { // Итератор на начало
        return tree.begin();
    }

    typename AVLTree<T>::Iterator end() { // Итератор на конец
        return tree.end();
    }

    bool operator==(Set<T>& other) { // Сравнение множеств
        return this->tree == other.tree;
    }

    bool operator!=(Set<T>& other) {
        return this->tree != other.tree;
    }

    bool findSubSet(Set<T>& other) { // Поиск подмножества
        for (T key : other.tree) {
            if (!this->contains(key)) {
                return false;
            }
        }
        return true;
    }

    int size() { // Количество элементов
        return tree.size();
    }

    friend std::ostream& operator<<(std::ostream& out, Set<T>& set) { // Вывод множества
        out << "{";
        auto it = set.tree.begin();
        while (it != set.tree.end()) {
            out << *it;
            ++it;
            if (it != set.tree.end()) {
                out << " ";
            }
        }
        out << "}" << std::endl;
        return out;
    }

    friend std::ostream& operator<<(std::ostream& out, Set<T>&& set) { // Вывод множества (rvalue)
        out << "{";
        auto it = set.tree.begin();
        while (it != set.tree.end()) {
            out << *it;
            ++it;
            if (it != set.tree.end()) {
                out << " ";
            }
        }
        out << "}" << std::endl;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Set<T>& set) { // Ввод множества
        T key;
        while (in >> key && key != -1) {
            set.insert(key);
        }
        return in;
    }
};

#endif //BST2_SET_H
