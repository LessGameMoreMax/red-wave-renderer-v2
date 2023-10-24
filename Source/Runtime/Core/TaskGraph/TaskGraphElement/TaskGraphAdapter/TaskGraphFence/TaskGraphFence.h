#ifndef TASK_GRAPH_FENCE_H
#define TASK_GRAPH_FENCE_H
#include "../TaskGraphAdapter.h"
namespace sablin{

class TaskGraphFence: public TaskGraphAdapter{
    friend class TaskGraphPipeline;
private:
    std::set<TaskGraphElement*> fence_elements_;
protected:
    explicit TaskGraphFence();
    virtual RStatus CheckSuitable() final;
    virtual void DumpElement(std::ostream& oss) final;
    virtual RStatus Run() final;

    TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER
    TASK_GRAPH_DECLARE_EVENT_MANAGER_WRAPPER
public:
    TaskGraphFence* WaitTaskGraphElement(TaskGraphElement* element);
    TaskGraphFence* WaitTaskGraphElements(const std::set<TaskGraphElement*>& elements);
    void Clear();
};

}
#endif
