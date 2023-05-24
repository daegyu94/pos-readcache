#include "src/read_cache/prefetch_dispatcher.h"

#include "src/bio/ubio.h"
#include "src/lib/block_alignment.h"
#include "src/io/general_io/translator.h"
#include "src/io/general_io/rba_state_service.h"
#include "src/io_scheduler/io_dispatcher.h"
#include "src/event_scheduler/event_scheduler.h"

namespace pos {
PrefetchDispatcher::PrefetchDispatcher(void) { }

PrefetchDispatcher::~PrefetchDispatcher(void) { }

void PrefetchDispatcher::Execute(PrefetchIOConfig *ioConfig) {
    EventSmartPtr event(new DummySubmitHandler(this, ioConfig));
    EventSchedulerSingleton::Instance()->EnqueueEvent(event);
}

void PrefetchDispatcher::Submit(PrefetchIOConfig *ioConfig) {
    UbioSmartPtr bio(new Ubio((void *) ioConfig->dst, SECTORS_PER_BLOCK, 
                ioConfig->meta->arrayId));

    bio->dir = UbioDir::Read;
    bio->SetLba(ioConfig->lba);
    bio->SetUblock(ioConfig->pba.arrayDev->GetUblock());
    CallbackSmartPtr callback(new DummyCallbackHandler(this, ioConfig));
    bio->SetCallback(callback);
    int ret = IODispatcherSingleton::Instance()->Submit(bio);
    if (!ret) {
        // io error?
    }
}


/* XXX: are submission and completion frontend event? */
DummySubmitHandler::DummySubmitHandler(PrefetchDispatcher *dispatcher,
            PrefetchIOConfig *ioConfig)
        : Event(true), dispatcher_(dispatcher), ioConfig_(ioConfig) { }

DummySubmitHandler::~DummySubmitHandler(void) { }

bool DummySubmitHandler::Execute(void) {
    dispatcher_->Submit(ioConfig_);
    return true;
}

DummyCallbackHandler::DummyCallbackHandler(PrefetchDispatcher *dispatcher,
            PrefetchIOConfig *ioConfig)
        : Callback(true), dispatcher_(dispatcher), ioConfig_(ioConfig) { }

DummyCallbackHandler::~DummyCallbackHandler(void) { }

bool DummyCallbackHandler::_DoSpecificJob(void) {
    RBAStateManager *rbaStateManager 
        = RBAStateServiceSingleton::Instance()->GetRBAStateManager(
            ioConfig_->meta->arrayId);
    BlkAddr blkRba = ChangeByteToBlock(ioConfig_->meta->rba);

    rbaStateManager->BulkReleaseOwnership(ioConfig_->meta->volumeId, blkRba, 1);
    
    delete ioConfig_;
    return true;
}
} // namespace pos
