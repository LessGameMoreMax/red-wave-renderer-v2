#ifndef TASK_GRAPH_DAEMON_MANAGER_H
#define TASK_GRAPH_DAEMON_MANAGER_H
#include "../TaskGraphManagerBase.h"
#include "TaskGraphDaemonBase.h"
#include "TaskGraphDaemon.h"
#include <set>
namespace sablin{

class TaskGraphDaemonManager: public TaskGraphDaemonBase,
                              public TaskGraphManagerBase<TaskGraphDaemon>{
    friend class TaskGraphPipeline;
private:
    std::set<TaskGraphDaemon*> daemons_;
protected:
    explicit TaskGraphDaemonManager();
    CLASS_NO_ALLOWED_COPY(TaskGraphDaemonManager)
    virtual ~TaskGraphDaemonManager();

    RStatus Setup();
    RStatus Exit();
    virtual RStatus Clear() override;

    virtual RStatus Add(TaskGraphDaemon* daemon) override;
    virtual RStatus Remove(TaskGraphDaemon* daemon) override;
    virtual size_t GetSize() const override;

    virtual TaskGraphDaemonManager* SetInterval(long interval) override;
    
};
}
#endif
