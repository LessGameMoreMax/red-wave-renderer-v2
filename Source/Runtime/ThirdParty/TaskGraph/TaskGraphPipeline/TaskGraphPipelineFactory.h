#ifndef TASK_GRAPH_PIPELINE_FACTORY_H
#define TASK_GRAPH_PIPELINE_FACTORY_H
#include "TaskGraphPipeline.h"
namespace sablin{

class TaskGraphPipelineFactory{
private:
    static std::mutex s_lock_;
    static std::list<TaskGraphPipeline*> s_pipeline_list_;
public:
    static TaskGraphPipeline* Create();
    static RStatus Remove(TaskGraphPipeline* pipeline);
    static RStatus Clear();
};

}
#endif
