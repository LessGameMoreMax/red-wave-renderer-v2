#ifndef TASK_GRAPH_PIPELINE_MANAGER_H
#define TASK_GRAPH_PIPELINE_MANAGER_H
#include "../../HAL/Runnable.h"
#include "../TaskGraphManagerBase.h"
#include "TaskGraphPipeline.h"
#include <list>
namespace sablin{

class TaskGraphPipelineManager: public Runnable,
                                public TaskGraphManagerBase<TaskGraphPipeline>{
private:
    bool is_init_;
    std::list<TaskGraphPipeline*> free_list_;
    std::list<TaskGraphPipeline*> used_list_;
    std::mutex lock_;
public:
    explicit TaskGraphPipelineManager();
    virtual ~TaskGraphPipelineManager();

    virtual RStatus Init() override;
    virtual RStatus Run() override;
    virtual RStatus Destroy() override;
    virtual RStatus Add(TaskGraphPipeline* ptr) override;
    virtual RStatus Clear() final;
    virtual bool Find(TaskGraphPipeline* ptr) const override;
    virtual RStatus Remove(TaskGraphPipeline* ptr) override;
    virtual size_t GetSize() const override;

    TaskGraphPipeline* Fetch();
    RStatus Release(TaskGraphPipeline* ptr);
};

}
#endif
