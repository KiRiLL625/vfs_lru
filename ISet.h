#ifndef L3_ISET_H
#define L3_ISET_H

template <typename T>
class ISet { // Интерфейс множества
public:
    virtual void insert(T key) = 0; // Вставка элемента
    virtual void remove(T key) = 0; // Удаление элемента
    virtual bool contains(T key) = 0; // Проверка наличия элемента
    //virtual ISet<T> operator+(ISet<T>& other) = 0;
    //virtual ISet<T> operator*(ISet<T>& other) = 0;
    //virtual ISet<T> operator-(ISet<T>& other) = 0;
    //virtual ISet<T> begin() = 0;
    //virtual ISet<T> end() = 0;
    virtual ~ISet() = default; // Виртуальный деструктор (для возможности наследования)
};

#endif //L3_ISET_H
