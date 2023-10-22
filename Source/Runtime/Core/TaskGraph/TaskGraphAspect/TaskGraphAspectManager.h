#ifndef TASK_GRAPH_ASPECT_MANAGER_H
#define TASK_GRAPH_ASPECT_MANAGER_H
#include "TaskGraphAspectBase.h"
#include "../TaskGraphManagerBase.h"
#include "TaskGraphAspect.h"
#include "../TaskGraphDefine.h"
namespace sablin{

class TaskGraphAspectManager: public TaskGraphAspectBase,
                              public TaskGraphManagerBase<TaskGraphAspect>{
    friend class TaskGraphElement;
private:
    std::vector<TaskGraphAspect*> aspect_arr_;
protected:
    explicit TaskGraphAspectManager();
    CLASS_NO_ALLOWED_COPY(TaskGraphAspectManager)
    virtual ~TaskGraphAspectManager();

    virtual RStatus Clear() override;

    RStatus PopLast();

    virtual size_t GetSize() const override;

    virtual RStatus Add(TaskGraphAspect* aspect) override;
    virtual TaskGraphAspectManager* SetName(const std::string& name) override;

    RStatus Reflect(const TaskGraphAspectType& type, const RStatus& cur_status = RStatus());
};

}
#endif
