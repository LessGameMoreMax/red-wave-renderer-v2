#ifndef GRAPH_TASK_THREAD_BASE_H
#define GRAPH_TASK_THREAD_BASE_H
#include "../../HAL/Runnable.h"
#include "../../HAL/RunnableThread.h"
#include "../../GenericPlatform/GenericPlatformAffinity.h"
#include "../../Container/AtomicQueue.h"
#include "../../Container/AtomicPriorityQueue.h"
#include "../GraphTask/GraphTask.h"
#include "../GraphTaskThreadPoolConfig.h"
#include <cstdint>
namespace sablin{

class GraphTaskThreadBase: public Runnable{
protected:
    bool is_done_;
    bool is_init_;
    bool is_running_;
    unsigned long graph_task_number_;

    AtomicQueue<GraphTask*>* graph_task_queue_;
    AtomicPriorityQueue<GraphTask*>* graph_task_priority_queue_;
    GraphTaskThreadPoolConfig* pool_config_;
    RunnableThread* thread_;

    explicit GraphTaskThreadBase();
    virtual ~GraphTaskThreadBase();

    virtual RStatus Setup() = 0;
    virtual RStatus Exit();
    virtual GraphTask* PopPoolTask();
    virtual bool PopPoolBatchTask(std::vector<GraphTask*>& tasks);
    void RunGraphTask(GraphTask* task);
    void RunGraphTasks(std::vector<GraphTask*>& tasks);

    virtual void ProcessGraphTask() = 0;
    virtual void ProcessGraphTasks() = 0;

    void Reset();

    RStatus LoopProcess();
    bool SetThreadPriority(const ThreadPriority thread_priority);
    bool SetThreadAffinity(const CpuSet& cpu_set);
};

}
#endif
