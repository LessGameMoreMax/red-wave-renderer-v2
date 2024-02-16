#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H
#include "../Core/Misc/MacroTools.h"
#include "../Container/ProducerConsumerQueue.h"
#include "WorkRunnable.h"
#include "WorkThreadPool.h"
namespace sablin{

class WorkQueue final{
    friend class WorkThreadRunnable;
private:
    inline static ProducerConsumerQueue<lenin::_WorkRunnable*>* work_queue_ = nullptr;
    inline static WorkThreadPool* work_thread_pool_ = nullptr;
private:
    explicit WorkQueue() = default;
public:
    CLASS_NO_ALLOWED_COPY(WorkQueue)
    ~WorkQueue();

    static void Initialize();
    static void Exit(bool should_wait = true);
};

}
#endif
