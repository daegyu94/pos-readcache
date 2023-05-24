#include "src/read_cache/prefetch_worker.h"

#include "src/include/memory.h"
#include "src/read_cache/prefetch_dispatcher.h"
#include "src/read_cache/read_cache.h"

#include <cassert>

namespace pos {
PrefetchWorker::PrefetchWorker() { }

PrefetchWorker::~PrefetchWorker() {
    if (threadPool_)
        delete threadPool_;
    if (dispatcher_)
        delete dispatcher_;
    if (service_)
        delete service_;
}

void PrefetchWorker::Enqueue(PrefetchMetaSmartPtr meta) {
    threadPool_->Enqueue([this, meta] {
                Work(meta);
            }); 
}

void PrefetchWorker::Initialize(int numThreads, std::string svrAddr) {
    threadPool_ = new PrefetchThreadPool(numThreads);
    dispatcher_ = new PrefetchDispatcher();
    service_ = new PrefetchServiceImpl();
    service_->Run(svrAddr, this);
}

bool PrefetchWorker::Work(PrefetchMetaSmartPtr meta) {
    RBAStateManager *rbaStateManager = 
        RBAStateServiceSingleton::Instance()->GetRBAStateManager(meta->arrayId);
    bool ownershipAcquired;
    BlkAddr blkRba = ChangeByteToBlock(meta->rba);
    PrefetchIOConfig *ioConfig;
    uintptr_t addr = 0;

    assert(meta != nullptr);
    
    ownershipAcquired = rbaStateManager->BulkAcquireOwnership(meta->volumeId, 
                blkRba, 1);
    /* write on the same rba, abandon prefetch */
    if (!ownershipAcquired) {
        return false;
    }
    
    ReadCacheSingleton::Instance()->Put(blkRba, addr);
    if (!addr) {
        return false; /* failed to allocate buffer */
    }
    printf("admit: blkRba=%lu, addr=%lu\n", blkRba, addr);
    /* ioConfig is deleted at completion */
    ioConfig = new PrefetchIOConfig(meta, addr); 
    dispatcher_->Execute(ioConfig);
    return true;
}
} // namespace pos
