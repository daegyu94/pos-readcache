#pragma once

#include "src/lib/singleton.h"
#include "src/read_cache/prefetch_dispatcher.h"
#include "src/read_cache/grpc_prefetch_server.h"

#include <atomic>
#include <vector>
#include <thread>
#include <functional>
#include <tbb/concurrent_queue.h>

namespace pos {
class PrefetchThreadPool {
public:
    PrefetchThreadPool(size_t numThreads) : stop_(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            threads_.emplace_back([this] {
                std::function<void()> task;
                while (!stop_) {
                    if (tasks_.try_pop(task)) {
                        task();
                    } else {
                        /* XXX: how long should sleep? */
                        std::this_thread::sleep_for(
                                std::chrono::microseconds(200));
                    }
                }
            });
        }
    }

    ~PrefetchThreadPool() {
        stop_ = true;
        for (std::thread& thread : threads_)
            thread.join();
    }

    template <class F>
    void Enqueue(F&& f) {
        tasks_.push(std::forward<F>(f));
    }

private:
    std::vector<std::thread> threads_;
    tbb::concurrent_queue<std::function<void()>> tasks_;
    std::atomic<bool> stop_;
};

class PrefetchWorker {
public:
    PrefetchWorker(void);
    ~PrefetchWorker(void);

    void Initialize(int, std::string);

    void Enqueue(PrefetchMetaSmartPtr);

private:
    bool Work(PrefetchMetaSmartPtr);

    /* TODO : create a thread pool and in charge of each initiator */
    //std::thread *thread_;
    PrefetchThreadPool *threadPool_;
    PrefetchDispatcher *dispatcher_;
    PrefetchServiceImpl *service_;
};
using PrefetchWorkerSingleton = Singleton<PrefetchWorker>;
} // namespace pos
