#ifndef TASK_GRAPH_PARAM_MANAGER_H
#define TASK_GRAPH_PARAM_MANAGER_H
#include "TaskGraphParam.h"
#include "../TaskGraphManagerBase.h"
#include <unordered_map>
namespace sablin{

class TaskGraphParamManager: public TaskGraphManagerBase<TaskGraphParam>{
private:
    std::unordered_map<std::string, TaskGraphParam*> param_map_;
    std::mutex lock_;
protected:
    explicit TaskGraphParamManager();
    ~TaskGraphParamManager();
    CLASS_NO_ALLOWED_COPY(TaskGraphParamManager)
    RStatus PreSetup();
    RStatus Setup();
    RStatus Exit();
    RStatus Clear();

    void ResetWithStatus(const RStatus& cur_status);
public:
    RStatus RemoveByKey(const std::string& key);
    RStatus GetKeys(std::vector<std::string>& keys);

    template <typename T, std::enable_if_t<std::is_base_of<TaskGraphParam, T>::value, int> = 0>
    RStatus Create(const std::string& key, bool backtrace = false){
        std::lock_guard<std::mutex> lg(lock_);
        auto result = param_map_.find(key);
        if(result != param_map_.end()){
            auto param = result->second;
            return (typeid(*param).name() == typeid(T).name())? RStatus():
                RStatus("Create [" + key + "] Param Duplicate");
        }
        T* ptr = new T();
        dynamic_cast<TaskGraphParam*>(ptr)->backtrace_enable_ = backtrace;
        dynamic_cast<TaskGraphParam*>(ptr)->key_ = key;
        param_map_.insert(std::pair<std::string, T*>(key, ptr));
        return RStatus();
    }

    template <typename T, std::enable_if_t<std::is_base_of<TaskGraphParam, T>::value, int> = 0>
    T* Get(const std::string& key){
        auto result = param_map_.find(key);
        if(result == param_map_.end()) return nullptr;
        return dynamic_cast<T*>(result->second);
    }
};

}
#endif
