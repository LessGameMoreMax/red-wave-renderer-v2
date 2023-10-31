#ifndef TASK_GRAPH_PERF_H
#define TASK_GRAPH_PERF_H
#include "../../../Core/HAL/RStatus.h"
namespace sablin{

class TaskGraphPipeline;

class TaskGraphPerf{
    friend TaskGraphPipeline;
protected:
    static RStatus Perf(TaskGraphPipeline* pipeline);
    static RStatus Inject(TaskGraphPipeline* pipeline);
    static RStatus Recover(TaskGraphPipeline* pipeline);
};

}
#endif
