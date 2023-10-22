#ifndef TASK_GRAPH_EVENT_MANAGER_WRAPPER_H
#define TASK_GRAPH_EVENT_MANAGER_WRAPPER_H
#include "TaskGraphEventManager.h"
namespace sablin{

#define TASK_GRAPH_DECLARE_EVENT_MANAGER_WRAPPER                                \
    void* SetTaskGraphEventManager(TaskGraphEventManager* event_manager){       \
        event_manager_ = event_manager;                                         \
        return this;                                                            \
    }                                                                           \
    RStatus Notify(const std::string& key, TaskGraphEventType event_type){      \
        RStatus status;                                                         \
        ASSERT_NO_STRING(event_manager_ != nullptr)                             \
        status += event_manager_->Trigger(key, event_type);                     \
        return status;                                                          \
    }                                                                           \

#define TASK_GRAPH_DECLARE_EVENT_MANAGER_WRAPPER_WITH_MEMBER                    \
private:                                                                        \
    TaskGraphEventManager* event_manager_ = nullptr;                            \
protected:                                                                      \
    TASK_GRAPH_DECLARE_EVENT_MANAGER_WRAPPER                                    \

}
#endif
