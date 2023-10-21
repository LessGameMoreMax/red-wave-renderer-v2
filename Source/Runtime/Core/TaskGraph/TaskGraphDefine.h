#ifndef TASK_GRAPH_DEFINE_H
#define TASK_GRAPH_DEFINE_H
#include <cstdint>
namespace sablin{

enum class TaskGraphScheduleType: uint32_t{
    kTaskGraphScheduleTypeUnique,
    kTaskGraphScheduleTypeShared
};

const static constexpr int32_t kRandomSeed = 0; // 0 means random totally!

enum class TaskGraphEventType: uint32_t{
    kTaskGraphEventTypeSync,
    kTaskGraphEventTypeAsync
};

}
#endif
