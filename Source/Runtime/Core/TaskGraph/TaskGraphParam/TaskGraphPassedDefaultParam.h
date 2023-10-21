#ifndef TASK_GRAPH_PASSED_DEFAULT_PARAM_H
#define TASK_GRAPH_PASSED_DEFAULT_PARAM_H
#include "TaskGraphPassedParam.h"
namespace sablin{

class TaskGraphPassedDefaultParam final: public TaskGraphPassedParam{
protected:
    virtual void Clone(TaskGraphPassedParam* param) final{}
};

}
#endif
