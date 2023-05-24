#pragma once

#include <mutex>

#include "src/read_cache/lru_cache.h"

namespace pos {
enum {
    kLRUPolicy,
    kMaxPolicy
};

template <typename KeyType, typename ValueType>
class FixedSizedCache {
public:
    FixedSizedCache(int num_shards, size_t max_size, int policy)
        : num_shards_{num_shards}, max_size_{max_size}
    {
        mtxes_ = new std::mutex[num_shards_]();

        /* TODO : make this code as template ? */
        caches_ = new ICachePolicy<KeyType, ValueType> *[num_shards_];
        for (int i = 0; i < num_shards_; i++) {
            switch (policy) {
                case kLRUPolicy:
                    caches_[i] = new LRUCache<KeyType, ValueType>(max_size_ / num_shards_);
                    break;
                default:
                    std::runtime_error("wrong policy");
            }
        }

    }

    ~FixedSizedCache() {
        delete[] mtxes_;
        for (int i = 0; i < num_shards_; i++) {
            delete caches_[i];
        }
        delete[] caches_;
    }

    void Put(const KeyType& key, const ValueType& value) {
        int id = GetShardID(key);
        std::lock_guard<std::mutex> lock(mtxes_[id]);
        caches_[id]->Put(key, value);
    }

    bool Get(const KeyType &key, ValueType &value) {
        int id = GetShardID(key);
        std::lock_guard<std::mutex> lock(mtxes_[id]);
        return caches_[id]->Get(key, value);
    }

    bool Delete(const KeyType &key, ValueType &value) {
        int id = GetShardID(key);
        std::lock_guard<std::mutex> lock(mtxes_[id]);
        return caches_[id]->Delete(key, value);
    }

    size_t Size(void) {
        return max_size_;
    }

    size_t Size(int id) {
        return caches_[id]->Size();
    }

private:
    int GetShardID(const KeyType &key) {
        if (num_shards_ == 1) {
            return 0;
        } else {
            return std::hash<KeyType>{}(key) % num_shards_;
        }
    }

    int num_shards_;
    size_t max_size_;
    mutable std::mutex *mtxes_;

    ICachePolicy<KeyType, ValueType> **caches_;
};
} // namespace pos
