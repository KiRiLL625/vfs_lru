#ifndef L3_LRUCACHE_H
#define L3_LRUCACHE_H

#include "ICache.h"
#include "Dictionary.h"
#include "ArraySequence.h"
#include <list>
#include <stdexcept>

template <typename Key, typename Value>
class LRUCache : public ICache<Key, Value> {
private:
    size_t capacity;  // Вместимость кэша
    Dictionary<Key, Value> dictionary;  // Словарь для хранения данных
    ArraySequence<Key> usageOrder;  // Последовательность ключей для отслеживания порядка использования
    size_t current_size;  // Текущий размер кэша

public:
    explicit LRUCache(size_t capacity) : capacity(capacity), current_size(0) {}

    void access(const Key& key, const Value& value) override {
        if (current_size >= capacity) {
            evict();  // Удаляем наименее недавно использованный элемент, если кэш переполнен
        }

        if (dictionary.contains_key(key)) {
            // Если элемент уже в кэше, обновляем его значение и перемещаем в конец
            dictionary[key] = value;
            moveToEnd(key);
        } else {
            // Если элемента нет в кэше, добавляем его
            dictionary.add(key, value);
            usageOrder.append(key);  // Добавляем ключ в последовательность
            ++current_size;
        }
    }

    bool contains(const Key& key) const override {
        return dictionary.contains_key(key);
    }

    const Value& get(const Key& key) const override {
        if (!contains(key)) {
            throw std::runtime_error("Key not found");
        }
        return dictionary.get(key);  // Возвращаем значение из словаря
    }

    size_t size() const override {
        return current_size;
    }

    void print() const override {
        std::cout << "Cache contents:\n";
        for (size_t i = 0; i < usageOrder.getLength(); ++i) {
            Key key = usageOrder[i];
            std::cout << "Key: " << key << ", Value: " << dictionary.get(key) << "\n";
        }
    }

    void clear() override {
        dictionary.clear();
        usageOrder.clear();
        current_size = 0;
    }

private:
    void evict() {
        // Удаляем наименее недавно использованный элемент (первый в последовательности)
        Key oldest_key = usageOrder.getFirst();
        dictionary.remove(oldest_key);
        usageOrder.remove(0);  // Удаляем из последовательности
        --current_size;
    }

    void moveToEnd(const Key& key) {
        // Перемещаем элемент в конец последовательности (чтобы он был недавно использован)
        size_t index = 0;
        for (size_t i = 0; i < usageOrder.getLength(); ++i) {
            if (usageOrder[i] == key) {
                index = i;
                break;
            }
        }
        usageOrder.remove(index);  // Убираем элемент из текущей позиции
        usageOrder.append(key);  // Добавляем в конец
    }
};

#endif // L3_LRUCACHE_H
