#ifndef TASK_GRAPH_EVENT_H
#define TASK_GRAPH_EVENT_H
#include "TaskGraphEventBase.h"
#include "../TaskGraphDefine.h"
#include "../TaskGraphParam/TaskGraphParamManagerWrapper.h"
namespace sablin{

class TaskGraphEvent: public TaskGraphEventBase{
    friend class TaskGraphEventManager;
private:
    RStatus Process(TaskGraphEventType event_type);
protected:
    virtual void Trigger(TaskGraphPassedParam* event_param_ptr) = 0;
    TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER
};

}
#endif
