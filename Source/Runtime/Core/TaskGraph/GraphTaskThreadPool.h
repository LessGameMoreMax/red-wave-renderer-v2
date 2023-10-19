#ifndef GRAPH_TASK_THREAD_POOL_H
#define GRAPH_TASK_THREAD_POOL_H
#include "../HAL/Runnable.h"
#include "../Container/AtomicQueue.h"
#include "../Container/AtomicPriorityQueue.h"
#include "GraphTaskThread/GraphTaskPrimaryThread.h"
#include "GraphTaskThread/GraphTaskSecondaryThread.h"
#include "GraphTask/GraphTask.h"
#include "GraphTask/GraphTaskGroup.h"
#include <list>
#include <map>
#include <future>
namespace sablin{

class GraphTaskThreadPool: public Runnable{
private:
    bool is_init_;
    int32_t cur_pool_id_;
    AtomicQueue<GraphTask*> graph_task_queue_;
    AtomicPriorityQueue<GraphTask*> graph_task_priority_queue_;
    std::vector<GraphTaskPrimaryThread*> primary_threads_;
    std::list<GraphTaskSecondaryThread*> secondary_threads_;
    GraphTaskThreadPoolConfig* config_;
    RunnableThread* monitor_thread_;
    Runnable* monitor_runnable_;
    std::map<int32_t, int32_t> thread_record_map_; // thread id => pool id
    std::mutex secondary_threads_lock_;
private:
    class MonitorRunnable: public Runnable{
    private:
        GraphTaskThreadPool* pool_ = nullptr;
    public:
        explicit MonitorRunnable(GraphTaskThreadPool* pool);
        virtual RStatus Run() override;
    };
protected:
    virtual int32_t Dispatch(int32_t origin_index);
public:
    explicit GraphTaskThreadPool(GraphTaskThreadPoolConfig* config = new GraphTaskThreadPoolConfig()) noexcept;
    virtual ~GraphTaskThreadPool() override;
    CLASS_NO_ALLOWED_COPY(GraphTaskThreadPool)

    virtual RStatus Init() override;
    int32_t GetThreadPoolId(int32_t thread_id);
    virtual RStatus Destroy() override;

    inline constexpr bool IsInit() const{
        return is_init_;
    }

    RStatus CreateSecondaryThread(uint32_t size);
    RStatus ReleaseSecondaryThread(uint32_t size);

    RStatus Submit(const GraphTaskGroup& task_group, long ttl = kMaxBlockTTL);
    RStatus Submit(const std::function<void()>& func, long ttl = kMaxBlockTTL,
            const std::function<void(RStatus)>& on_finished = nullptr);

    template <typename Func>
    auto Commit(const Func& func, int32_t index = kDefaultGraphTaskStrategy)
        -> std::future<decltype(std::declval<Func>()())> {
        std::packaged_task<decltype(std::declval<Func>()())()> task(func);
        std::future<decltype(std::declval<Func>()())> result(task.get_future());

        int32_t real_index = Dispatch(index);
        if(real_index >= 0 && real_index < config_->primary_thread_size_){
            primary_threads_[real_index]->PushGraphTask(new GraphTask(std::move(task)));
        }else if(real_index == kLongTimeTaskStrategy){
            graph_task_priority_queue_.Push(new GraphTask(std::move(task)), kLongTimeTaskStrategy);
        }else{
            graph_task_queue_.Push(new GraphTask(std::move(task)));
        }
        return result;
    }

    template <typename Func>
    auto CommitWithPriority(const Func& func, int32_t priority)
        -> std::future<decltype(std::declval<Func>()())> {
        std::packaged_task<decltype(std::declval<Func>()())()> task(func);
        std::future<decltype(std::declval<Func>()())> result(task.get_future());
        if(secondary_threads_.empty()) CreateSecondaryThread(1);
        graph_task_priority_queue_.Push(new GraphTask(std::move(task)), priority);
        return result;
    }
};

}
#endif
