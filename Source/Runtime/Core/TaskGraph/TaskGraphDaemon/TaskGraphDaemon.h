#ifndef TASK_GRAPH_DAEMON_H
#define TASK_GRAPH_DAEMON_H
#include "TaskGraphDaemonBase.h"
#include "../TaskGraphTimer.h"
namespace sablin{

class TaskGraphDaemon: public TaskGraphDaemonBase{
private:
    TaskGraphTimer timer_;
private:
    RStatus Setup();
    RStatus Exit();
protected:
    virtual void DaemonTask(TaskGraphPassedParam* param) = 0;
    long GetInterval() const;
};

}
#endif
