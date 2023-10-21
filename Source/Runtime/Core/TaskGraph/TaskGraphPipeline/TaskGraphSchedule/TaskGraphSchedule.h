#ifndef TASK_GRAPH_SCHEDULE_H
#define TASK_GRAPH_SCHEDULE_H
#include "../../GraphTaskThreadPool.h"
#include "../../TaskGraphDefine.h"
namespace sablin{

class TaskGraphSchedule{
private:
    GraphTaskThreadPool* unique_thread_pool_;
    GraphTaskThreadPool* shared_thread_pool_;
    TaskGraphScheduleType schedule_type_;
    GraphTaskThreadPoolConfig pool_config_;
private:
    explicit TaskGraphSchedule();
    ~TaskGraphSchedule();

    RStatus MakeType(GraphTaskThreadPool* thread_pool);
    GraphTaskThreadPool* GetThreadPool();

    RStatus Setup();
    RStatus Exit();
};

}
#endif
