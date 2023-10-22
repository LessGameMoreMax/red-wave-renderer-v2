#ifndef TASK_GRAPH_ELEMENT_H
#define TASK_GRAPH_ELEMENT_H
#include "../TaskGraphInterface/TaskGraphDescInfoInterface.h"
#include "../../HAL/Runnable.h"
#include "TaskGraphElementDefine.h"
#include <atomic>
#include <unordered_map>
#include "../TaskGraphParam/TaskGraphPassedParam.h"
#include "../TaskGraphAspect/TaskGraphAspectManager.h"
#include "../GraphTaskThreadPool.h"
#include "../TaskGraphPipeline/TaskGraphPerf/TaskGraphPerfDefine.h"
#include "../TaskGraphParam/TaskGraphParamManagerWrapper.h"
#include "../TaskGraphEvent/TaskGraphEventManagerWrapper.h"
#include "TaskGraphElementRelation.h"
namespace sablin{

class TaskGraphElement: public Runnable,
                        public TaskGraphDescInfoInterface{
private:
    bool done_;
    bool linkable_;
    bool visible_;
    bool is_init_;
    TaskGraphElementTimeoutStrategy timeout_strategy_;
    TaskGraphElementType element_type_;
    std::atomic<TaskGraphElementState> cur_state_;

    size_t loop_;
    int32_t level_;
    int32_t binding_index_;
    long timeout_;

    std::unordered_map<std::string, TaskGraphPassedParam*> local_params_;
    TaskGraphAspectManager* aspect_manager_;
    GraphTaskThreadPool* thread_pool_;
    TaskGraphPerfInfo* perf_info_;
    long trigger_times_;

    std::atomic<size_t> left_depend_;
    std::set<TaskGraphElement*> run_before_;
    std::set<TaskGraphElement*> dependence_;
    TaskGraphElement* belong_;

    std::future<RStatus> async_result_;
    std::mutex yield_mutex_;
    std::condition_variable yield_cv_;
private:
    TASK_GRAPH_DECLARE_EVENT_MANAGER_WRAPPER_WITH_MEMBER
    TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER_WITH_MEMBER

    virtual RStatus BeforeRun();
    virtual RStatus AfterRun();

    inline bool IsRunnable() const{
        return left_depend_ <= 0 && !done_;
    }
    inline bool IsLinkable() const{
        return linkable_;
    }
    inline bool IsAsync() const{
        return timeout_ != kTaskGraphDefaultElementTimeout;
    }

    RStatus DoAspect(const TaskGraphAspectType& aspect_type,
                const RStatus& cur_status = RStatus());

    virtual RStatus SetElementInfo(const std::set<TaskGraphElement*>& depend_elements,
                const std::string& name, size_t loop, TaskGraphParamManager* param_manager,
                TaskGraphEventManager* event_manager);

    RStatus FatProcessor(const TaskGraphFunctionType& function_type);

    TaskGraphElement* SetThreadPool(GraphTaskThreadPool* thread_pool);

    virtual void Dump(std::ostream& oss);
    void DumpEdge(std::ostream& oss, TaskGraphElement* src, TaskGraphElement* dst,
                const std::string& label = STRING_NULL);
    virtual void DumpElement(std::ostream& oss);
    void DumpElementHeader(std::ostream& oss);
    void DumpPerfInfo(std::ostream& oss);

    inline void CheckYield(){
        std::unique_lock<std::mutex> lk(yield_mutex_);
        yield_cv_.wait(lk, [this]{
            return cur_state_ == TaskGraphElementState::kYield;
        });
    }

    virtual bool IsSerializable();
    RStatus PopLastAspect();
    RStatus AsyncRun();
    RStatus GetAsyncResult();
    virtual RStatus CheckSuitable();
protected:
    explicit TaskGraphElement();
    CLASS_NO_ALLOWED_COPY(TaskGraphElement)
    virtual ~TaskGraphElement();

    virtual RStatus PrepareRun();
    virtual bool IsHold();
    virtual bool IsMatch();
    bool IsTimeout() const;

    virtual RStatus Crashed(const std::string& str);
    int32_t GetThreadIndex();
    virtual int32_t GetBindingIndex();
    RStatus BuildRelation(TaskGraphElementRelation& relation);

    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphPassedParam, T>::value, int> = 0>
    T* GetElementParam(const std::string& key){
        auto iter = local_params_.find(key);
        return dynamic_cast<T*>((iter != local_params_.end())? iter->second: nullptr);
    }
public:
};

}
#endif
