#pragma once

#include <unordered_map>
#include <list>
#include <mutex>

#include "src/read_cache/i_cache_policy.h"

namespace pos {
template<typename KeyType, typename ValueType>
class LRUCache : public ICachePolicy<KeyType, ValueType> {
public:
    LRUCache(size_t max_size) : max_size_(max_size) {}
    ~LRUCache() {}

    // Insert a new key/value pair into the cache.
    void Put(const KeyType& key, const ValueType& value) override {

        auto iter = cache_map_.find(key);
        if (iter != cache_map_.end()) {
            // Key already exists, update the value and move it to the front of the list.
            iter->second->second = value;
            cache_list_.splice(cache_list_.begin(), cache_list_, iter->second);
            return;
        }

        // Key does not exist, insert a new element at the front of the list.
        cache_list_.emplace_front(key, value);
        cache_map_[key] = cache_list_.begin();

        // If the cache size is now greater than max_size_, evict the oldest element.
        if (cache_map_.size() > max_size_) {
            auto last = cache_list_.end();
            last--;
            cache_map_.erase(last->first);
            cache_list_.pop_back();
        }
    }

    // Get the value for the given key.
    // Returns true if the key was found, and sets 'value' to the corresponding value.
    bool Get(const KeyType& key, ValueType& value) override {
        auto iter = cache_map_.find(key);
        if (iter == cache_map_.end()) {
            return false;
        }

        // Move the key to the front of the list.
        cache_list_.splice(cache_list_.begin(), cache_list_, iter->second);

        value = iter->second->second;
        return true;
    }

    // Remove the key/value pair for the given key from the cache.
    bool Delete(const KeyType& key, ValueType& value) override {
        auto iter = cache_map_.find(key);
        if (iter != cache_map_.end()) {
            value = iter->second->second;
            cache_list_.erase(iter->second);
            cache_map_.erase(iter);
            return true;
        } else {
            return false;
        }
    }

    size_t Size(void) const override {
        return cache_map_.size();
    }

private:
    size_t max_size_;
    std::list<std::pair<KeyType, ValueType>> cache_list_;
    std::unordered_map<KeyType, typename std::list<std::pair<KeyType, ValueType>>::iterator> cache_map_;
};
} // namespace pos
