#ifndef GRAPH_TASK_THREAD_BASE_H
#define GRAPH_TASK_THREAD_BASE_H
#include "../Runnable.h"
#include "../../GenericPlatform/GenericPlatformAffinity.h"
#include "../../Container/AtomicQueue.h"
#include "../../Container/AtomicPriorityQueue.h"
#include <cstdint>
namespace sablin{

class GraphTaskThreadBase: public Runnable{
protected:
    bool is_done_;
    bool is_init_;
    bool is_running_;
    ThreadType thread_type_;
    unsigned long graph_task_number_;

};

}
#endif
