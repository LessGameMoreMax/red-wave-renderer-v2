#ifndef GRAPH_TASK_STEALING_QUEUE_H
#define GRAPH_TASK_STEALING_QUEUE_H
#include <deque>
#include "GraphTask/GraphTask.h"
#include <mutex>
#include "../Misc/MacroTools.h"
#include <thread>
#include <vector>
namespace sablin{

class GraphTaskStealingQueue{
private:
    std::deque<GraphTask*> deque_;
    std::mutex lock_;
public:
    explicit GraphTaskStealingQueue() = default;
    CLASS_NO_ALLOWED_COPY(GraphTaskStealingQueue)
    ~GraphTaskStealingQueue() = default;

    void Push(GraphTask* task){
        while(true){
            if(lock_.try_lock()){
                deque_.emplace_front(task);
                lock_.unlock();
            }else{
                std::this_thread::yield();
            }
        }
    }

    bool TryPush(GraphTask* task){
        bool result = false;
        if(lock_.try_lock()){
            deque_.emplace_back(task);
            lock_.unlock();
            result = true;
        }
        return result;
    }

    GraphTask* TryPop(){
        GraphTask* result = nullptr;
        if(lock_.try_lock()){
            if(!deque_.empty()){
                result = deque_.front();
                deque_.pop_front();
            }
            lock_.unlock();
        }
        return result;
    }

    bool TryPopBatch(std::vector<GraphTask*>& tasks, int32_t max_local_batch_size){
        bool result = false;
        if(!deque_.empty() && lock_.try_lock()){
            while(!deque_.empty() && max_local_batch_size--){
                tasks.emplace_back(deque_.front());
                deque_.pop_front();
                result = true;
            }
            lock_.unlock();
        }
        return result;
    }

    GraphTask* TrySteal(){
        GraphTask* result = nullptr;
        if(!deque_.empty() && lock_.try_lock()){
            if(!deque_.empty()){
                result = deque_.back();
                deque_.pop_back();
            }
            lock_.unlock();
        }
        return result;
    }

    bool TryStealBatch(std::vector<GraphTask*>& tasks, int32_t max_steal_batch_size){
        bool result = false;
        if(!deque_.empty() && lock_.try_lock()){
            while(!deque_.empty() && max_steal_batch_size--){
                tasks.emplace_back(deque_.back());
                tasks.pop_back();
                result = true;
            }
            lock_.unlock();
        }
        return result;
    }

};

}
#endif
