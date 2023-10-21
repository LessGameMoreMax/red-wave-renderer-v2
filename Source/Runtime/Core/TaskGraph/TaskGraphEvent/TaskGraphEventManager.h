#ifndef TASK_GRAPH_EVENT_MANAGER_H
#define TASK_GRAPH_EVENT_MANAGER_H
#include "../TaskGraphManagerBase.h"
#include "TaskGraphEvent.h"
#include "TaskGraphEventBase.h"
#include "../TaskGraphParam/TaskGraphPassedDefaultParam.h"
namespace sablin{

class TaskGraphEventManager: public TaskGraphEventBase,
                             public TaskGraphManagerBase<TaskGraphEvent>{
private:
    std::unordered_map<std::string, TaskGraphEvent*> event_map_;
protected:
    explicit TaskGraphEventManager();
    virtual ~TaskGraphEventManager();
    CLASS_NO_ALLOWED_COPY(TaskGraphEventManager)

    virtual RStatus Setup() override;
    virtual RStatus Exit() override;
    virtual RStatus Clear() final;
    TaskGraphEventBase* SetThreadPool(GraphTaskThreadPool* thread_pool) override;
public:
    RStatus Trigger(const std::string& key, TaskGraphEventType event_type);
    
    template<typename T, typename TEventParam = TaskGraphPassedDefaultParam,
        std::enable_if_t<std::is_base_of<TaskGraphEvent, T>::value, int> = 0,
        std::enable_if_t<std::is_base_of<TaskGraphPassedParam, TEventParam>::value, int> = 0>
    RStatus CreateWithParam(const std::string& key, TEventParam* param = nullptr){
        auto result = event_map_.find(key);
        if(result != event_map_.end()){
            return RStatus("Event [" + key + "] Has Been Register!");
        }
        TaskGraphEvent* event = new T();
        event->SetTaskGraphParamManager(param_manager_);

        // if(event->event_param_ptr_ != nullptr){
        //     delete event->event_param_ptr_;
        //     event->event_param_ptr_ = nullptr;
        // }
        ASSERT_NO_STRING(event->event_param_ptr_ == nullptr)
        if(param != nullptr){
            event->event_param_ptr_ = new TEventParam();
            event->event_param_ptr_->Clone(param);
        }
        event_map_[key] = event;
        return RStatus();
    }

};

}
#endif
