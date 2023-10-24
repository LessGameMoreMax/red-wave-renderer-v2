#ifndef TASK_GRAPH_ASPECT_BASE_H
#define TASK_GRAPH_ASPECT_BASE_H
#include "../TaskGraphInterface/TaskGraphDescInfoInterface.h"
#include "../TaskGraphParam/TaskGraphPassedParam.h"
#include "../TaskGraphParam/TaskGraphParamManager.h"
#include "../TaskGraphEvent/TaskGraphEventManager.h"
#include "../TaskGraphRandom.h"
#include "../TaskGraphEvent/TaskGraphEventManagerWrapper.h"
#include "../TaskGraphParam/TaskGraphParamManagerWrapper.h"
namespace sablin{

class TaskGraphAspectBase: public TaskGraphDescInfoInterface{
    friend class TaskGraphElement;
private:
    TaskGraphPassedParam* aspect_param_ptr_;
    TaskGraphParamManager* param_manager_;
    TaskGraphEventManager* event_manager_;
protected:
    explicit TaskGraphAspectBase();
    CLASS_NO_ALLOWED_COPY(TaskGraphAspectBase)
    virtual ~TaskGraphAspectBase();

    TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER
    TASK_GRAPH_DECLARE_EVENT_MANAGER_WRAPPER

    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphPassedParam, T>::value, int> = 0>
    T* GetParam(){
        ASSERT_NO_STRING(aspect_param_ptr_ != nullptr)
        T* param = nullptr;
        if(typeid(*param).name() == typeid(T).name()){
            param = dynamic_cast<T*>(aspect_param_ptr_);
        }
        return param;
    }

    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphPassedParam, T>::value, int> = 0>
    TaskGraphAspectBase* SetAspectParam(T* param){
        if(param == nullptr) return this;
        if(aspect_param_ptr_ != nullptr){
            delete aspect_param_ptr_;
            aspect_param_ptr_ = nullptr;
        }
        aspect_param_ptr_ = new T();
        aspect_param_ptr_->Clone(static_cast<T*>(param));
        return this;
    }
};

}
#endif
