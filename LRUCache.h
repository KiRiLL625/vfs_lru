#ifndef L3_LRUCACHE_H
#define L3_LRUCACHE_H

#include "ICache.h"
#include "Dictionary.h"
#include "ArraySequence.h"
#include <stdexcept>

template <typename Key, typename Value>
class LRUCache : public ICache<Key, Value> {
private:
    size_t capacity;
    Dictionary<Key, Value> dictionary;
    ArraySequence<Key> usageOrder;
    Dictionary<Key, size_t> keyPositions;
    size_t current_size;

public:
    explicit LRUCache(size_t capacity) : capacity(capacity), current_size(0) {}

    void access(const Key& key, const Value& value) override {
        if (dictionary.contains_key(key)) {
            dictionary[key] = value;
            moveToEnd(key);
        } else {
            if (current_size >= capacity) {
                evict();
            }
            dictionary.add(key, value);
            usageOrder.append(key);
            keyPositions.add(key, usageOrder.getLength() - 1);  // <--- Запоминаем позицию
            ++current_size;
        }
    }

    bool contains(const Key& key) const override {
        return dictionary.contains_key(key);
    }

    Value& get(const Key& key) override {
        if (!contains(key)) {
            throw std::runtime_error("Key not found");
        }
        moveToEnd(key);
        return dictionary.get(key);
    }

    const Value& get(const Key& key) const override {
        if (!contains(key)) {
            throw std::runtime_error("Key not found");
        }
        return dictionary.get(key);
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
        keyPositions.clear();
        current_size = 0;
    }

private:
    void evict() {
        if (usageOrder.getLength() == 0) {
            return;
        }
        Key oldest_key = usageOrder.getFirst();
        dictionary.remove(oldest_key);
        keyPositions.remove(oldest_key);
        usageOrder.remove(0);
        --current_size;

        // Обновляем позиции всех оставшихся элементов
        for (size_t i = 0; i < usageOrder.getLength(); ++i) {
            keyPositions[usageOrder[i]] = i;
        }
    }

    void moveToEnd(const Key& key) {
        size_t index = keyPositions[key];
        usageOrder.remove(index);
        usageOrder.append(key);

        // Обновляем позиции ключей
        keyPositions[key] = usageOrder.getLength() - 1;
        for (size_t i = index; i < usageOrder.getLength() - 1; ++i) {
            keyPositions[usageOrder[i]] = i;
        }
    }
};

#endif // L3_LRUCACHE_H
