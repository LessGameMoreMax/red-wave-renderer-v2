#ifndef TASK_GRAPH_DAEMON_BASE_H
#define TASK_GRAPH_DAEMON_BASE_H
#include "../TaskGraphInterface/TaskGraphDescInfoInterface.h"
#include "../TaskGraphParam/TaskGraphParamManager.h"
#include "../TaskGraphEvent/TaskGraphEventManager.h"
#include "../TaskGraphParam/TaskGraphParamManagerWrapper.h"
#include "../TaskGraphEvent/TaskGraphEventManagerWrapper.h"
namespace sablin{

class TaskGraphDaemonBase: public TaskGraphDescInfoInterface{
    friend class TaskGraphDaemon;
private:
    TaskGraphParamManager* param_manager_;
    TaskGraphEventManager* event_manager_;
    TaskGraphPassedParam* daemon_param_ptr_;
    long interval_;    // Unit: ms
protected:
    explicit TaskGraphDaemonBase();
    virtual ~TaskGraphDaemonBase();
    CLASS_NO_ALLOWED_COPY(TaskGraphDaemonBase)

    TASK_GRAPH_DECLARE_EVENT_MANAGER_WRAPPER
    TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER

    virtual TaskGraphDaemonBase* SetInterval(long interval);

    template <typename DParam, 
             std::enable_if_t<std::is_base_of<TaskGraphPassedParam, DParam>::value, int> = 0>
    TaskGraphDaemonBase* SetDaemonParam(DParam* param){
        if(param == nullptr) return this;
        if(daemon_param_ptr_ != nullptr){
            delete daemon_param_ptr_;
            daemon_param_ptr_ = nullptr;
        }
        daemon_param_ptr_ = new DParam();
        daemon_param_ptr_->Clone(static_cast<DParam*>(param));
        return this;
    }
};



}
#endif
