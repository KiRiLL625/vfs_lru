#ifndef L3_LRUCACHE_H
#define L3_LRUCACHE_H

#include "ICache.h"
#include "Dictionary.h"
#include <list>
#include <iostream>

// Реализация LRUCache
template <typename Key, typename Value>
class LRUCache : public ICache<Key, Value> {
private:
    struct CacheItem { // Структура для хранения элемента кэша
        Key key;
        Value value;
    };

    size_t max_size; // Максимальный размер кэша
    Dictionary<Key, typename std::list<CacheItem>::iterator> cache_map; // Словарь для быстрого доступа к элементам (ключ - ключ, значение - итератор на элемент в списке)
    std::list<CacheItem> cache_list; // Список элементов кэша

public:
    explicit LRUCache(size_t max_size) : max_size(max_size), cache_map(max_size) {} // Конструктор с параметром

    void access(const Key& key, const Value& value) override { // Добавление элемента в кэш
        if (cache_map.contains_key(key)) { // Если элемент уже есть в кэше
            auto it = cache_map.get(key); // Получаем итератор на элемент
            cache_list.splice(cache_list.begin(), cache_list, it); // Перемещаем элемент в начало списка
            it->value = value; // Обновляем значение
        } else { // Если элемента нет в кэше
            if (cache_list.size() >= max_size) { // Если кэш переполнен
                auto last = cache_list.back(); // Получаем последний элемент
                cache_map.remove(last.key); // Удаляем его из словаря
                cache_list.pop_back(); // Удаляем его из списка
            }
            cache_list.push_front({key, value}); // Добавляем новый элемент в начало списка
            cache_map.add(key, cache_list.begin()); // Добавляем новый элемент в словарь
        }
    }

    bool contains(const Key& key) const override { // Проверка наличия элемента в кэше
        return cache_map.contains_key(key); // Проверяем наличие ключа в словаре
    }

    const Value& get(const Key& key) const override { // Получение значения по ключу
        if (!cache_map.contains_key(key)) { // Если ключ не найден
            throw std::runtime_error("Key not found in cache");
        }
        return cache_map.get(key)->value; // Возвращаем значение по ключу
    }

    size_t size() const override { // Количество элементов в кэше
        return cache_list.size();
    }

    void clear() override { // Очистка кэша
        for(const auto& item : cache_list) { // Перебираем все элементы кэша
            cache_map.remove(item.key); // Удаляем элемент из словаря
        }
        cache_list.clear(); // Очищаем список
    }

    void print() const override { // Вывод кэша на экран
        for (const auto& item : cache_list) { // Перебираем все элементы кэша
            std::cout << item.key << ": " << item.value << std::endl;
        }
    }
};

#endif //L3_LRUCACHE_H
