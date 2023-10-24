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

enum class TaskGraphAspectType: uint32_t{
    kBeginInit = 0,
    kFinishInit = 1,
    kBeginRun = 2,
    kFinishRun = 3,
    kBeginDestroy = 4,
    kFinishDestroy = 5,
    kEnterCrashed = 99
};

enum class TaskGraphFunctionType: uint32_t{
    kInit = 1,
    kRun = 2,
    kDestroy = 3
};

enum class TaskGraphEngineType: uint32_t{
    kStatic = 1,
    kDynamic = 2
};

}
#endif
