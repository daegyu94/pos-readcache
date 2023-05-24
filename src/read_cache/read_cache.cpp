#include "src/include/memory.h"

#include "src/read_cache/read_cache.h"

namespace pos {
void ReadCache::Initialize(size_t cacheSize) {
    memoryManager_ =  MemoryManagerSingleton::Instance();
    /*
     * block size incurs more write invalidations
     * sector size incurs too much entries
     */
    BufferInfo info = {
        .owner = typeid(this).name(),
        .size = BLOCK_SIZE,
        .count = cacheSize / info.size
    };

    bufferPool_ = memoryManager_->CreateBufferPool(info);
    assert(bufferPool_ != nullptr);
    
    int numShards = 1;
    size_t maxSize = info.count;
    int policy = kLRUPolicy;
    cache_ = new FixedSizedCache<uint64_t, uintptr_t>(numShards, maxSize, policy);
}

ReadCache::ReadCache() { }

ReadCache::~ReadCache() {
    memoryManager_->DeleteBufferPool(bufferPool_);
    if (cache_)
        delete cache_;
}
} // namespace pos
