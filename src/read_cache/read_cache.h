#pragma once

#include "src/lib/singleton.h"
#include "src/include/address_type.h"
#include "src/resource_manager/buffer_pool.h"
#include "src/resource_manager/memory_manager.h"
#include "src/read_cache/fixed_sized_cache.h"

namespace pos {
class ReadCache {
    public:
        ReadCache(void);
        ~ReadCache(void);
        
        void Initialize(size_t);

        void Put(BlkAddr blkRba, uintptr_t &dst) {
            uintptr_t addr = (uintptr_t) bufferPool_->TryGetBuffer();
            if (addr) {
                cache_->Put(blkRba, addr);
                dst = addr;
            }
        }

        bool Get(BlkAddr blkRba, uintptr_t &addr) {
            return cache_->Get(blkRba, addr); 
        }

        bool Delete(BlkAddr blkRba, uintptr_t &addr) {
            return cache_->Delete(blkRba, addr);
        }

        void ReturnBuffer(uintptr_t addr) {
            if (addr) {
                bufferPool_->ReturnBuffer((void *) addr); 
            } 
        }

        BufferPool *GetBufferPool(void) {
            return bufferPool_;
        } 

        FixedSizedCache<uint64_t, uintptr_t> *GetCache(void) {
            return cache_;
        }

    private:
        /* memory pool for cached block */
        MemoryManager *memoryManager_;
        BufferPool *bufferPool_;
        
        FixedSizedCache<uint64_t, uintptr_t> *cache_;
};
using ReadCacheSingleton = Singleton<ReadCache>;
} // namespace pos
