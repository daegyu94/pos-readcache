#pragma once

#include "src/read_cache/prefetch_meta.h"

#include "src/event_scheduler/callback.h"
#include "src/event_scheduler/event.h"

#include <atomic>

namespace pos {
class PrefetchDispatcher {
public:
	PrefetchDispatcher();
	~PrefetchDispatcher();

	void Execute(PrefetchIOConfig *);
	void Submit(PrefetchIOConfig *);
};

class DummySubmitHandler : public Event {
public:
    DummySubmitHandler(PrefetchDispatcher *, PrefetchIOConfig *);
    ~DummySubmitHandler(void) override;

    bool Execute(void);

private:
    PrefetchDispatcher *dispatcher_;
    PrefetchIOConfig *ioConfig_;
};

class DummyCallbackHandler : public Callback {
public:
    DummyCallbackHandler(PrefetchDispatcher *, PrefetchIOConfig *);
    ~DummyCallbackHandler(void) override;

private:
    bool _DoSpecificJob(void);

    PrefetchDispatcher *dispatcher_;
    PrefetchIOConfig *ioConfig_;
};
} // namespace pos
