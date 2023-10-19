#ifndef GRAPH_TASK_THREAD_POOL_DEFINE_H
#define GRAPH_TASK_THREAD_POOL_DEFINE_H
#include <thread>
#include "../GenericPlatform/GenericPlatformAffinity.h"
namespace sablin{

static const uint32_t kCpuNumber = std::thread::hardware_concurrency(); 
static constexpr long kMaxBlockTTL = 1999999999;                           // Max Block Time, Unit: ms
static constexpr uint32_t kDefaultRingBufferSize = 1024;                   // The Default Size of Ring Buffer

static constexpr int32_t kDefaultGraphTaskStrategy = -1;                   // The Default Thread Dispatch Strategy
static constexpr int32_t kPoolGraphTaskStrategy = -2;                      // The Dispatch Strategy In Pool
static constexpr int32_t kLongTimeTaskStrategy = -101;                     // The Dispatch Strategy Of Long Time

static constexpr uint32_t kDefaultPrimaryThreadSize = 0;                   // The Default Size of Primary Thread
static constexpr uint32_t kDefaultSecondaryThreadSize = 8;                 // The Default Size of Secondary Thread
static constexpr uint32_t kMaxThreadSize = 16;                             // Max Thread Size
static constexpr uint32_t kMaxTaskStealRange = 2;                          // The Range of Steal
static constexpr bool kBatchTaskEnable = false;                            // Enable The Batch Task
static constexpr uint32_t kMaxLocalBatchSize = 2;                          // Max Size Of Batch Local Task
static constexpr uint32_t kMaxPoolBatchSize = 2;                           // Max Size Of Batch Pool Task
static constexpr uint32_t kMaxStealBatchSize = 2;                          // Max Size Of Batch Steal Task
static constexpr uint32_t kSecondaryThreadTTL = 10;                        // Secondary Thread ttl, Unit: ms
static constexpr bool kMonitorEnable = false;                              // Enable The Monitor Program
static constexpr uint32_t kMonitorSpan = 5;                                // The Moninor Program Work Interval, Unit: s
static constexpr long kQueueEmptyInterval = 3;                             // When Queue Is Empty, The Time Of Wait(Only Secondary Thread), Unit: ms
static constexpr ThreadPriority kPrimaryThreadPriority = ThreadPriority::kThreadPriorityNormal;
static constexpr ThreadPriority kSecondaryThreadPriority = ThreadPriority::kThreadPriorityNormal;
static constexpr ThreadPriority kMonitorThreadPriority = ThreadPriority::kThreadPriorityNormal;


}
#endif
