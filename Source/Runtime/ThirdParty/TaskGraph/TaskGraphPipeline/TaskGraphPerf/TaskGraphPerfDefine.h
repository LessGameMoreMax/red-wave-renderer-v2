#ifndef TASK_GRAPH_PERF_DEFINE_H
#define TASK_GRAPH_PERF_DEFINE_H
#include <cstdint>
namespace sablin{

struct TaskGraphPerfInfo{
    uint32_t loop_ = 0;
    double first_start_ts_ = 0.0; // Unit: ms
    double last_finish_ts_ = 0.0; // Unit: ms
    double accu_cost_ts_ = 0.0; // Unit: ms
};

}
#endif
