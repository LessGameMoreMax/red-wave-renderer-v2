#ifndef TASK_GRAPH_EVENT_BASE_H
#define TASK_GRAPH_EVENT_BASE_H
#include "../TaskGraphInterface/TaskGraphDescInfoInterface.h"
#include "../GraphTaskThreadPool.h"
#include "../TaskGraphParam/TaskGraphPassedParam.h"
#include "../TaskGraphParam/TaskGraphParamManager.h"
#include "../TaskGraphRandom.h"
namespace sablin{

class TaskGraphEventBase: public TaskGraphDescInfoInterface{
protected:
    GraphTaskThreadPool* thread_pool_;
    TaskGraphPassedParam* event_param_ptr_;
    TaskGraphParamManager* param_manager_;
protected:
    explicit TaskGraphEventBase():
        thread_pool_(nullptr),
        event_param_ptr_(nullptr),
        param_manager_(nullptr){
        session_ = TaskGraphRandom<>::GenerateSession("event");
    }

    virtual ~TaskGraphEventBase(){
        delete event_param_ptr_;
    }

    virtual TaskGraphEventBase* SetThreadPool(GraphTaskThreadPool* pool){
        thread_pool_ = pool;
        return this;
    }

    virtual RStatus Setup(){
        return RStatus();
    }

    virtual RStatus Exit(){
        return RStatus();
    }
};

}
#endif
