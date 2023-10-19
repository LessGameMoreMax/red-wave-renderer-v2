#ifndef GRAPH_TASK_PRIMARY_THREAD_H
#define GRAPH_TASK_PRIMARY_THREAD_H
#include "GraphTaskThreadBase.h"
#include "../GraphTaskStealingQueue.h"
#include "../../GenericPlatform/GenericPlatformProcess.h"
#include "../../Misc/MacroDefine.h"
#include <algorithm>

namespace sablin{

class GraphTaskPrimaryThread: public GraphTaskThreadBase{
    friend class GraphTaskThreadPool;
private:
    int32_t pool_id_;
    int32_t steal_range_;
    GraphTaskStealingQueue primary_queue_;
    GraphTaskStealingQueue secondary_queue_;
    std::vector<GraphTaskPrimaryThread*>* pool_threads_;
protected:
    explicit GraphTaskPrimaryThread():
        pool_id_(-1),
        steal_range_(0),
        pool_threads_(nullptr){
    }

    // Before Init()!
    RStatus SetGraphTaskThreadPoolInfo(int32_t pool_id, AtomicQueue<GraphTask*>* pool_task_queue,
            std::vector<GraphTaskPrimaryThread*>* pool_threads, GraphTaskThreadPoolConfig* config){
        ASSERT_NO_STRING(is_init_ == false)
        ASSERT_NO_STRING(pool_task_queue != nullptr && pool_threads != nullptr && config != nullptr)

        pool_id_ = pool_id;
        graph_task_queue_ = pool_task_queue;
        pool_threads_ = pool_threads;
        pool_config_ = config;
        return RStatus();
    }

    virtual RStatus Init() override{
        ASSERT_NO_STRING(is_init_ == false)
        ASSERT_NO_STRING(pool_config_ != nullptr)

        steal_range_ = pool_config_->CalcStealRange();
        thread_ = PlatformProcess::CreateNativeThread(this, 
                "GraphTaskPrimaryThread", pool_config_->primary_thread_priority_, 
                ThreadType::kThreadTypeGraphTaskPrimaryThread);
        thread_->SetupThread(CpuSet());
        is_init_ = true;
        return RStatus();
    }

    virtual RStatus Run() override{
#ifdef DEBUG
        ASSERT_NO_STRING(is_init_ == true)
        ASSERT_NO_STRING(pool_threads_ != nullptr)
        ASSERT_NO_STRING(!std::any_of(pool_threads_->begin(), pool_threads_->end(),
                    [](GraphTaskPrimaryThread* pt){ return pt == nullptr; }))
#endif
        return LoopProcess();
    }

    GraphTask* PopGraphTask(){
        GraphTask* result = nullptr;
        result = primary_queue_.TryPop();
        if(result != nullptr) return result;
        return secondary_queue_.TryPop();
    }

    bool PopBatchGraphTask(std::vector<GraphTask*>& tasks){
        bool result = primary_queue_.TryPopBatch(tasks, pool_config_->max_pool_batch_size_);
        int32_t left_size = (int32_t)pool_config_->max_pool_batch_size_ - tasks.size();
        if(left_size > 0){
            result |= (secondary_queue_.TryPopBatch(tasks, left_size));
        }
        return result;
    }

    void PushGraphTask(GraphTask* task){
        while(!primary_queue_.TryPush(task) && !secondary_queue_.TryPush(task))
            std::this_thread::yield();
    }

    GraphTask* StealGraphTask(){
        if(pool_threads_->size() == pool_config_->primary_thread_size_) [[likely]]{
            for(int32_t i = 0;i < steal_range_; ++i){
                int32_t steal_index = (pool_id_ + i + 1) % pool_config_->primary_thread_size_;
                GraphTask* task = (*pool_threads_)[steal_index]->secondary_queue_.TrySteal();
                if(task != nullptr) return task;

                task = (*pool_threads_)[steal_index]->primary_queue_.TrySteal();
                if(task != nullptr) return task;
            }
        }
        return nullptr;
    }

    bool StealBatchGraphTask(std::vector<GraphTask*>& tasks){
        if(pool_threads_->size() != pool_config_->primary_thread_size_) [[unlikely]]
            return false;

        bool result = false;
        for(int32_t i = 0;i < steal_range_ && !result; ++i){
            int32_t steal_index = (pool_id_ + i + 1) % pool_config_->primary_thread_size_;
            bool result = (*pool_threads_)[steal_index]->secondary_queue_.TryStealBatch(tasks,
                    pool_config_->max_steal_batch_size_);
            int32_t left_size = pool_config_->max_steal_batch_size_ - tasks.size();
            if(left_size > 0){
                result |= (*pool_threads_)[steal_index]->primary_queue_.TryStealBatch(tasks, left_size);
            }
        }
        return result;
    }

    virtual void ProcessGraphTask() override{
        GraphTask* task = PopGraphTask();
        if(task != nullptr){
            RunGraphTask(task);
            delete task;
            return;
            
        }

        task = PopPoolTask();
        if(task != nullptr){
            RunGraphTask(task);
            delete task;
            return;
        }

        task = StealGraphTask();
        if(task != nullptr){
            RunGraphTask(task);
            delete task;
        }else{
            std::this_thread::yield();
        }
    }

    virtual void ProcessGraphTasks() override{
        std::vector<GraphTask*> tasks;
        if(PopBatchGraphTask(tasks) || PopPoolBatchTask(tasks) || StealBatchGraphTask(tasks)){
            RunGraphTasks(tasks);
            for(GraphTask* task: tasks)
                delete task;
        }else{
            std::this_thread::yield();
        }
    }



};

}
#endif
