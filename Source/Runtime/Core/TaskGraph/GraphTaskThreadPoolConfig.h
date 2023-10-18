#ifndef GRAPH_TASK_THREAD_POOL_CONFIG_H
#define GRAPH_TASK_THREAD_POOL_CONFIG_H
#include "GraphTaskThreadPoolDefine.h"
#include "../HAL/RStatus.h"
#include "../Misc/MacroTools.h"
#include "../Math/MathTools.h"
namespace sablin{

struct GraphTaskThreadPoolConfig{
    friend class GraphTaskPrimaryThread;
    friend class GraphTaskSecondaryThread;
    uint32_t primary_thread_size_ = kDefaultPrimaryThreadSize;
    uint32_t secondary_thread_size_ = kDefaultSecondaryThreadSize;
    uint32_t max_thread_size_ = kMaxThreadSize;
    uint32_t max_task_steal_range_ = kMaxTaskStealRange;
    uint32_t max_local_batch_size_ = kMaxLocalBatchSize;
    uint32_t max_pool_batch_size_ = kMaxPoolBatchSize;
    uint32_t max_steal_batch_size_ = kMaxStealBatchSize;
    uint32_t secondary_thread_ttl_ = kSecondaryThreadTTL;
    uint32_t monitor_span_ = kMonitorSpan;
    long queue_empty_interval_ = kQueueEmptyInterval;
    ThreadPriority primary_thread_priority_ = kPrimaryThreadPriority;
    ThreadPriority secondary_thread_priority_ = kSecondaryThreadPriority;
    bool batch_task_enable_ = kBatchTaskEnable;
    bool monitor_enable_ = kMonitorEnable;

    RStatus GraphTaskThreadPoolConfigCheck() const{
        if(primary_thread_size_ < 0 || secondary_thread_size_ < 0){
            return RStatus("Thread Size Can Not Less Than Zero!", STRING_CODE_LOCATION);
        }

        if(primary_thread_size_ + secondary_thread_size_ > max_thread_size_){
            return RStatus("Primary Thread Plus Secondary Thread Greater Than Max Thread Size!", STRING_CODE_LOCATION);
        }

        if(monitor_enable_ && monitor_span_ <= 0){
            return RStatus("Monitor Span Can Not Less Than ZERO!", STRING_CODE_LOCATION);
        }
        return RStatus();
    }

protected:
    int32_t CalcStealRange() const{
        return Min<int32_t>(max_task_steal_range_, static_cast<int32_t>(primary_thread_size_) - 1);
    }
};

}
#endif
