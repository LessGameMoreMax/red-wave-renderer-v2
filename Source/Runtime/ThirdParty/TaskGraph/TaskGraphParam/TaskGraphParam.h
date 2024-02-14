#ifndef TASK_GRAPH_PARAM_H
#define TASK_GRAPH_PARAM_H
#include <string>
#include "../../Core/Container/SerialUniqueArray.h"
#include "../TaskGraphLock/TaskGraphSpinLock.h"
#include "../../Core/HAL/RStatus.h"
#include <shared_mutex>
namespace sablin{

class TaskGraphParam{
    friend class TaskGraphParamManager;
private:
    bool backtrace_enable_;
    std::string key_;
    SerialUniqueArray<std::string> backtrace_;
    TaskGraphSpinLock backtrace_lock_;
protected:
    virtual RStatus PreSetup(){
        return RStatus();
    }

    virtual RStatus Setup(){
        return RStatus();
    }
    virtual void Reset(const RStatus& cur_status) {}

    virtual RStatus Exit(){
        return RStatus();
    }
public:
    std::shared_mutex param_shared_lock_;

    explicit TaskGraphParam();
    virtual ~TaskGraphParam() = default;

    RStatus GetBackTrace(std::vector<std::string>& traces);
    void AddBackTrace(const std::string& trace);
    void CleanBackTrace();
    const std::string& GetKey() const;
};

}
#endif
