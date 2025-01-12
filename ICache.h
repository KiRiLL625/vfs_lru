#ifndef L3_ICACHE_H
#define L3_ICACHE_H

#include <cstdlib>

// Интерфейс ICache
template <typename Key, typename Value>
class ICache {
public:
    virtual void access(const Key& key, const Value& value) = 0; // Добавление элемента в кэш
    virtual bool contains(const Key& key) const = 0; // Проверка наличия элемента в кэше
    virtual const Value& get(const Key& key) const = 0; // Получение значения по ключу
    virtual size_t size() const = 0; // Количество элементов в кэше
    virtual void print() const = 0; // Вывод кэша на экран
    virtual void clear() = 0; // Очистка кэша
    virtual ~ICache() = default; // Виртуальный деструктор (для возможности наследования)
};

#endif //L3_ICACHE_H
