#ifndef GRAPH_TASK_SECONDARY_THREAD_H
#define GRAPH_TASK_SECONDARY_THREAD_H
#include "GraphTaskThreadBase.h"
#include "../../GenericPlatform/GenericPlatformProcess.h"
#include "../../Math/MathTools.h"
namespace sablin{

class GraphTaskSecondaryThread: public GraphTaskThreadBase{
    friend class GraphTaskThreadPool;
private:
    int32_t cur_ttl_;
protected:
    virtual RStatus Init() override{
        ASSERT_NO_STRING(is_init_ == false)
        ASSERT_NO_STRING(pool_config_ != nullptr)
        cur_ttl_ = pool_config_->secondary_thread_ttl_;
        thread_ = PlatformProcess::CreateNativeThread(this, "GraphTaskSecondaryThread",
                pool_config_->secondary_thread_priority_, 
                ThreadType::kThreadTypeGraphTaskSecondaryThread);
        thread_->SetupThread(CpuSet());
        is_init_ = true;
        return RStatus();
    }

    RStatus SetGraphTaskThreadPoolInfo(AtomicQueue<GraphTask*>* pool_task_queue,
            AtomicPriorityQueue<GraphTask*>* pool_priority_task_queue,
            GraphTaskThreadPoolConfig* config){
        ASSERT_NO_STRING(is_init_ == false)
        ASSERT_NO_STRING(pool_task_queue != nullptr && pool_priority_task_queue != nullptr)
        ASSERT_NO_STRING(config != nullptr)

        graph_task_queue_ = pool_task_queue; 
        graph_task_priority_queue_ = pool_priority_task_queue;
        pool_config_ = config;
        return RStatus();
    }

    virtual RStatus Run() override{
        ASSERT_NO_STRING(is_init_ == true)
        return LoopProcess();
    }

    virtual void ProcessGraphTask() override{
        GraphTask* task;
        task = PopPoolTask();
        if(task != nullptr){
            RunGraphTask(task);
            delete task;
        }else{
            WaitRunTask(pool_config_->queue_empty_interval_);
        }
    }

    virtual void ProcessGraphTasks() override{
        std::vector<GraphTask*> tasks;
        if(PopPoolBatchTask(tasks)){
            RunGraphTasks(tasks);
            for(GraphTask* task: tasks)
                delete task;
        }else{
            WaitRunTask(pool_config_->queue_empty_interval_);
        }
    }

    void WaitRunTask(long ms){
        GraphTask* task = graph_task_queue_->PopTimeOut(ms);
        if(task != nullptr){
            (*task)();
            delete task;
        }
    }

    bool Freeze(){
        if(is_running_){
            ++cur_ttl_;
            cur_ttl_ = Min(cur_ttl_, (int32_t)pool_config_->secondary_thread_ttl_);
        }else{
            --cur_ttl_;
        }
        return cur_ttl_ <= 0 && is_done_;
    }


public:
    explicit GraphTaskSecondaryThread(): cur_ttl_(0){}
};

}
#endif
