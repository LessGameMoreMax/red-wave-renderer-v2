#ifndef TASK_GRAPH_TEMPLATE_DAEMON_H
#define TASK_GRAPH_TEMPLATE_DAEMON_H
#include "TaskGraphDaemon.h"
namespace sablin{

template<typename... Args>
class TaskGraphTemplateDaemon: public TaskGraphDaemon{};
}
#endif
