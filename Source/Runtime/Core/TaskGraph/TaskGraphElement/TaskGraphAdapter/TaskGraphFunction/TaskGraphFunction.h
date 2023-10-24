#ifndef TASK_GRAPH_FUNCTION_H
#define TASK_GRAPH_FUNCTION_H
#include "../TaskGraphAdapter.h"
namespace sablin{

class TaskGraphFunction: public TaskGraphAdapter{
    friend class TaskGraphPipeline;
private:
    std::function<RStatus()> init_function_;
    std::function<RStatus()> run_function_;
    std::function<RStatus()> destroy_function_;
private:
    explicit TaskGraphFunction();
    virtual RStatus Init() final;
    virtual RStatus Run() final;
    virtual RStatus Destroy() final;
public:
    TaskGraphFunction* SetFunction(const TaskGraphFunctionType& type,
            const std::function<RStatus()>& func);
    TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER
    TASK_GRAPH_DECLARE_EVENT_MANAGER_WRAPPER
};

}
#endif
