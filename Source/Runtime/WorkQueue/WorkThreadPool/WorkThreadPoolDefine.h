#ifndef WORK_THREAD_POOL_DEFINE_H
#define WORK_THREAD_POOL_DEFINE_H
#include <cstdint>
namespace sablin{

static constexpr uint32_t kWorkThreadMaxPoolSize = 8;
static constexpr uint32_t kWorkThreadPrimaryCacheSize = 4;
static constexpr uint32_t kWorkThreadPrimaryStealSize = 2;
static constexpr uint32_t kWorkThreadSecondaryBusySize = 1;
static constexpr uint32_t kWorkThreadSecondaryVeryBusySize = 2;
static constexpr uint32_t kWorkQueueWorkPoolBusySize = 2;
static constexpr uint32_t kWorkThreadMonitorMinScanMilli = 500;
static constexpr uint32_t kWorkThreadMonitorMaxScanMilli = 1000;

}
#endif
