#ifndef BST2_BST_H
#define BST2_BST_H

#include <functional>
#include <iterator>

template <typename T>
class Node { // Класс узла дерева
public:
    T key; // Ключ узла (значение)
    int height; // Высота узла
    Node* left; // Левый потомок
    Node* right; // Правый потомок

    explicit Node(T key) { // Конструктор узла
        this->key = key; //устанавливаем все значения по умолчанию
        this->height = 1;
        this->left = nullptr;
        this->right = nullptr;
    }
};

template <typename T>
class AVLTree { // Класс дерева (сбалансированного дерева поиска)
private:
    Node<T>* root; // Корень дерева

    int height(Node<T>* N) { // Получаем высоту узла
        if (N == nullptr) // Если узел пустой, то возвращаем 0
            return 0;
        return N->height; // Возвращаем высоту узла
    }

    int max(int a, int b) { // Получаем максимальное значение (для выявления необходимости балансировки)
        return (a > b) ? a : b;
    }

    Node<T>* rightRotate(Node<T>* y) { // Правый поворот (против часовой стрелки)
        Node<T>* x = y->left; // x это левый потомок y
        Node<T>* T2 = x->right; // Поддерево T2 становится левым потомком y
        x->right = y; // y становится правым потомком x
        y->left = T2; // Поддерево T2 становится правым потомком y
        y->height = max(height(y->left), height(y->right)) + 1; // Обновляем высоту y
        x->height = max(height(x->left), height(x->right)) + 1; // Обновляем высоту x
        return x;
    }

    Node<T>* leftRotate(Node<T>* x) { // Левый поворот (по часовой стрелке)
        Node<T>* y = x->right; // y это правый потомок x
        Node<T>* T2 = y->left; // Поддерево T2 становится правым потомком x
        y->left = x; // x становится левым потомком y
        x->right = T2; // Поддерево T2 становится правым потомком x
        x->height = max(height(x->left), height(x->right)) + 1; // Обновляем высоту x
        y->height = max(height(y->left), height(y->right)) + 1; // Обновляем высоту y
        return y;
    }

    int getBalance(Node<T>* N) { // Получаем баланс узла
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right); // Возвращаем разницу высот левого и правого поддеревьев
    }

    Node<T>* insert(Node<T>* node, T key) { // Вставка узла с ключом key в дерево
        if (node == nullptr)
            return(new Node<T>(key));
        if (key < node->key)
            node->left  = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right)); // Обновляем высоту текущего узла

        int balance = getBalance(node); // Получаем баланс текущего узла

        //Если узел не сбалансирован, то есть 4 случая вращения
        // Левый Левый случай
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Правый Правый случай
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Левый Правый случай
        if (balance > 1 && key > node->left->key) {
            node->left =  leftRotate(node->left);
            return rightRotate(node);
        }

        // Правый Левый случай
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node<T>* minValueNode(Node<T>* node) { // Находим узел с минимальным значением ключа
        Node<T>* current = node;
        while (current->left != nullptr) // Пока есть левый потомок
            current = current->left;
        return current;
    }

    Node<T>* remove(Node<T>* root, T key) { // Удаляем узел с ключом key из дерева
        if (root == nullptr)
            return root;
        if ( key < root->key )
            root->left = remove(root->left, key);
        else if(key > root->key)
            root->right = remove(root->right, key);
        else {
            // Узел с одним или без детей
            if( (root->left == nullptr) || (root->right == nullptr) ) {
                Node<T> *temp = root->left ? root->left : root->right; // Получаем потомка узла
                if(temp == nullptr) { // Узел без детей
                    temp = root;
                    root = nullptr;
                }
                else // Узел с одним ребенком
                    *root = *temp;
                delete temp; // Удаляем узел
            }
            else { // Узел с двумя детьми
                Node<T>* temp = minValueNode(root->right); // Находим узел с минимальным значением ключа в правом поддереве
                root->key = temp->key; // Копируем значение узла
                root->right = remove(root->right, temp->key); // Удаляем узел
            }
        }
        if (root == nullptr)
            return root;
        // Обновляем высоту текущего узла
        root->height = 1 + max(height(root->left), height(root->right));
        // Получаем баланс текущего узла
        int balance = getBalance(root);
        // Если узел не сбалансирован, то есть 4 случая вращения
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left =  leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    // Поиск узла с ключом key
    bool contains(Node<T>* root, T key) {
        if (root == nullptr)
            return false;
        if (key < root->key)
            return contains(root->left, key);
        else if (key > root->key)
            return contains(root->right, key);
        return true;
    }

public:
    AVLTree() {
        root = nullptr;
    }

    void insert(T key) {
        root = insert(root, key);
    }

    void remove(T key) {
        root = remove(root, key);
    }

    bool contains(T key) {
        return contains(root, key);
    }

    class Iterator { // Итератор для обхода дерева
    private:
        Node<T>* current; // Текущий узел
        ArraySequence<Node<T>*> nodes; // Стек для хранения узлов

        void pushLeftmost(Node<T>* node) { // Перемещаемся в самый левый узел
            while (node != nullptr) {
                nodes.append(node);
                node = node->left;
            }
        }

    public:
        using iterator_category = std::forward_iterator_tag; // Указываем категорию итератора
        using value_type = T; // Указываем типы
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(Node<T>* root) { // Конструктор итератора
            pushLeftmost(root); // Перемещаемся в самый левый узел
            current = nodes.getLength() != 0 ? nodes.getLast() : nullptr; // Устанавливаем текущий узел
        }

        T operator*() { // Получаем значение текущего узла
            return current->key;
        }

        Iterator& operator++() { // Переход к следующему узлу
            if (nodes.getLength() != 0) {
                Node<T>* node = nodes.getLast();
                nodes.removeLast();
                pushLeftmost(node->right);
                current = nodes.getLength() != 0 ? nodes.getLast() : nullptr;
            }
            return *this;
        }

        bool operator!=(const Iterator& it) const { // Проверка на неравенство
            return current != it.current;
        }

        bool operator==(const Iterator& it) const { // Проверка на равенство
            return current == it.current;
        }

    };

    Iterator begin() { // Начало итератора
        return Iterator(root);
    }

    Iterator end() { // Конец итератора
        return Iterator(nullptr);
    }

    int size() { // Количество элементов
        int count = 0;
        for (T key : *this) {
            count++;
        }
        return count;
    }

    void deleteNode(Node<T>* node) { // Удаление узла
        if (node == nullptr) {
            return;
        }
        deleteNode(node->left);
        deleteNode(node->right);
        delete node;
    }

    ~AVLTree() {
        deleteNode(root);
    }

    friend void map(std::function<void(T)> f, Node<T>* node) { // Применение функции к каждому узлу
        if (node == nullptr) {
            return;
        }
        map(f, node->left);
        f(node->key);
        map(f, node->right);
    }

};

#endif //BST2_BST_H
