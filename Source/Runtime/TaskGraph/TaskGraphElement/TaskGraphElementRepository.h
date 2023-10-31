#ifndef TASK_GRAPH_ELEMENT_REPOSITORY_H
#define TASK_GRAPH_ELEMENT_REPOSITORY_H
#include "TaskGraphElement.h"
namespace sablin{

class TaskGraphElementRepository{
    friend class TaskGraphPerf;
    friend class TaskGraphPipeline;
private:
    std::set<TaskGraphElement*> elements_;
    TaskGraphElementState cur_state_;
    std::set<TaskGraphElement*> async_elements_;
public:
    explicit TaskGraphElementRepository();
    ~TaskGraphElementRepository();

    void Insert(TaskGraphElement* ptr);
    bool Find(TaskGraphElement* ptr) const;
    TaskGraphElementRepository* SetThreadPool(GraphTaskThreadPool* ptr);

    RStatus PreSetup();
    RStatus Reset();
    RStatus PullAllState(const TaskGraphElementState& state);

    bool IsCancelState() const;
    RStatus Setup();
    RStatus Exit();
};

}
#endif
