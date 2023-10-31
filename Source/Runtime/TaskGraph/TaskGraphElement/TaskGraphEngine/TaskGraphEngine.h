#ifndef TASK_GRAPH_ENGINE_H
#define TASK_GRAPH_ENGINE_H
#include "../../../Core/HAL/Runnable.h"
#include "../../GraphTaskThreadPool.h"
#include "../TaskGraphElementSorter.h"
namespace sablin{

class TaskGraphEngine: public Runnable{
    friend class TaskGraphElementManager;
protected:
    GraphTaskThreadPool* thread_pool_;
    uint32_t total_element_size_;
    int32_t schedule_strategy_;
protected:
    explicit TaskGraphEngine();

    virtual RStatus Setup(const std::set<TaskGraphElement*, TaskGraphElementSorter>& elements) = 0;
    virtual RStatus AfterRunCheck() = 0;
    int32_t CalcIndex(TaskGraphElement* element);

};

}
#endif
