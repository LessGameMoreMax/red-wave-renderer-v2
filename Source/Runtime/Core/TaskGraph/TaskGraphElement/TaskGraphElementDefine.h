#ifndef TASK_GRAPH_ELEMENT_DEFINE_H
#define TASK_GRAPH_ELEMENT_DEFINE_H
#include <cstdint>
namespace sablin{

static constexpr std::size_t kTaskGraphDefaultLoopTimes = 1;
static constexpr int32_t kTaskGraphDefaultElementLevel = 0;
static constexpr int32_t kTaskGraphDefaultBindingIndex = -1;
static constexpr long kTaskGraphDefaultElementTimeout = 0; // Unit: ms

enum class TaskGraphElementTimeoutStrategy: uint32_t{
    kAsError = 1,
    kHoldByPipeline = 2,
    kNoHold = 3
};

enum class TaskGraphElementType{
    kElement = 0x00000000,
    kNode = 0x00010000,
    kGroup = 0x00020000,
    kCluster = 0x00020001,
    kRegion = 0x00020002,
    kCondition = 0x00020004,
    kSome = 0x00020008,
    kMultiCondition = 0x00020014,
    kAdapter = 0x00040000,
    kFunction = 0x00040001,
    kSingleton = 0x00040002,
    kFence = 0x00040004,
    kCoordinator = 0x00040008
};

enum class TaskGraphElementState{
    kCreate = 0x0000,
    kNormal = 0x1000,
    kCancel = 0x1001,
    kYield = 0x1002,
    kTimeout = 0x1010
};

using TaskGraphPipelineState = TaskGraphElementState;

}
#endif
