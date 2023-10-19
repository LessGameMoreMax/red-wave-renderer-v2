#ifndef GENERIC_PLATFORM_AFFINITY_H
#define GENERIC_PLATFORM_AFFINITY_H
#include <cstdint>
#include <set>
namespace sablin{

#define THREAD_AFFINITY_MASK_1(a) ((1 << a))

enum class ThreadPriority: int32_t{
    kThreadPriorityIdle,
    kThreadPriorityLowest,
    kThreadPriorityBelowNormal,
    kThreadPriorityNormal,
    kThreadPriorityAboveNormal,
    kThreadPriorityHighest,
    kThreadPriorityTimeCritical,
    
    Count // 7
};

struct CpuSet{
    std::set<uint16_t> cpu_set_;
};

enum class ThreadType: uint32_t{
    kThreadTypeNative,
    kThreadTypeGraphTaskPrimaryThread,
    kThreadTypeGraphTaskSecondaryThread,
    kThreadTypeGraphTaskMonitorThread,
    kThreadTypeNone
};

}
#endif
