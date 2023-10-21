#ifndef TASK_GRAPH_PASSED_PARAM_H
#define TASK_GRAPH_PASSED_PARAM_H
#include <unordered_map>
namespace sablin{

class TaskGraphPassedParam{
public:
    virtual ~TaskGraphPassedParam() = default;
    virtual void Clone(TaskGraphPassedParam* param) = 0;
};

}
#endif
