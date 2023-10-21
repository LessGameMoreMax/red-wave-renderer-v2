#ifndef TASK_GRAPH_PARAM_MANAGER_WRAPPER_H
#define TASK_GRAPH_PARAM_MANAGER_WRAPPER_H
#include "TaskGraphParam.h"
#include "../../Debug/Assertion.h"
#include "TaskGraphParamManager.h"
namespace sablin{

#define TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER                                               \
    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphParam, T>::value, int> = 0> \
    RStatus CreateParam(const std::string& key, bool backtrace = false){                       \
        ASSERT_NO_STRING(param_manager_ != nullptr)                                            \
        return param_manager_->Create<T>(key, backtrace);                                      \
    }                                                                                          \
    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphParam, T>::value, int> = 0> \
    T* GetParam(const std::string& key){                                                       \
        ASSERT_NO_STRING(param_manager_ != nullptr)                                            \
        auto param = param_manager_->Get<T>(key);                                              \
        if(param != nullptr){                                                                  \
            param->AddBackTrace(name_.empty() ? session_ : name_);                             \
        }                                                                                      \
        return param;                                                                          \
    }                                                                                          \
    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphParam, T>::value, int> = 0> \
    T* GetParamWithNoEmpty(const std::string& key){                                            \
        T* param = GetParam<T>(key);                                                           \
        ASSERT_NO_STRING(param != nullptr)                                                     \
        return param;                                                                          \
    }                                                                                          \
    RStatus RemoveParam(const std::string& key){                                               \
        ASSERT_NO_STRING(param_manager_ != nullptr)                                            \
        return param_manager_->RemoveByKey(key);                                               \
    }                                                                                          \
    RStatus GetParamKeys(std::vector<std::string>& keys){                                      \
        ASSERT_NO_STRING(param_manager_ != nullptr)                                            \
        return param_manager_->GetKeys(keys);                                                  \
    }                                                                                          \
private:                                                                                       \
    void* SetTaskGraphParamManager(TaskGraphParamManager* param_manager){                      \
        param_manager_ = param_manager;                                                        \
        return this;                                                                           \
    }                                                                                          \
protected:                                                                                     \

#define TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER_WITH_MEMBER                                   \
private:                                                                                       \
    TaskGraphParamManager* param_manager_ = nullptr;                                           \
protected:                                                                                     \
    TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER                                                   \

}
#endif
