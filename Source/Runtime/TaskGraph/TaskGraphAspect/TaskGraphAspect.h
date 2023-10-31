#ifndef TASK_GRAPH_ASPECT_H
#define TASK_GRAPH_ASPECT_H
#include "TaskGraphAspectBase.h"
namespace sablin{

class TaskGraphAspect: public TaskGraphAspectBase{
public:
    virtual RStatus BeginInit();
    virtual void FinishInit(const RStatus& cur_status);
    
    virtual RStatus BeginRun();
    virtual void FinishRun(const RStatus& cur_status);

    virtual RStatus BeginDestroy();
    virtual void FinishDestroy(const RStatus& cur_status);

    virtual void EnterCrashed();
};

}
#endif
