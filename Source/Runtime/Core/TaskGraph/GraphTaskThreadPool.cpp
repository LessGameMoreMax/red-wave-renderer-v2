#include "GraphTaskThreadPool.h"
namespace sablin{

GraphTaskThreadPool::GraphTaskThreadPool(GraphTaskThreadPoolConfig* config) noexcept:
    is_init_(false), cur_pool_id_(0), config_(config){}

GraphTaskThreadPool::~GraphTaskThreadPool(){
    config_->monitor_enable_ = false;
    if(monitor_thread_->IsJoinable()){
        monitor_thread_->Join();
    }
    delete monitor_thread_;
    delete monitor_runnable_;
    Destroy();
    delete config_;
}

RStatus GraphTaskThreadPool::Init(){
    if(is_init_) return RStatus();
    RStatus status;
    monitor_runnable_ = new MonitorRunnable(this);
    monitor_thread_ = PlatformProcess::CreateNativeThread(monitor_runnable_, "GraphTaskThreadPoolMonitorThread",
            config_->monitor_thread_priority_, ThreadType::kThreadTypeGraphTaskMonitorThread);
    monitor_thread_->SetupThread(CpuSet());
    thread_record_map_.clear();
    primary_threads_.reserve(config_->primary_thread_size_);
    for(uint32_t i = 0;i != config_->primary_thread_size_; ++i){
        GraphTaskPrimaryThread* ptr = new GraphTaskPrimaryThread();
        ptr->SetGraphTaskThreadPoolInfo(i, &graph_task_queue_, &primary_threads_, config_);
        status += ptr->Init();

        thread_record_map_[ptr->thread_->GetThreadId()] = i;
        primary_threads_.emplace_back(ptr);
    }
    
    ASSERT_NO_STRING(status.IsOk())

    status = CreateSecondaryThread(config_->secondary_thread_size_);

    ASSERT_NO_STRING(status.IsOk())

    is_init_ = true;
    return status;
}

int32_t GraphTaskThreadPool::GetThreadPoolId(int32_t thread_id){
    auto iter = thread_record_map_.find(thread_id);
    if(iter == thread_record_map_.end()) return -1;
    return iter->second;
}

RStatus GraphTaskThreadPool::Destroy(){
    RStatus status;
    if(!is_init_) return status;

    for(GraphTaskPrimaryThread* pt: primary_threads_)
        status += pt->Destroy();

    ASSERT_NO_STRING(status.IsOk())

    for(GraphTaskPrimaryThread* pt: primary_threads_)
        delete pt;
    primary_threads_.clear();

    for(GraphTaskSecondaryThread* st: secondary_threads_)
        delete st;
    secondary_threads_.clear();
    thread_record_map_.clear();
    is_init_ = false;
    return status;
}

RStatus GraphTaskThreadPool::CreateSecondaryThread(uint32_t size){
    RStatus status;
    uint32_t left_size = config_->max_thread_size_ - config_->primary_thread_size_ - secondary_threads_.size();
    left_size = Min(left_size, size);

    std::lock_guard<std::mutex> lg(secondary_threads_lock_);
    for(uint32_t i = 0;i != left_size; ++i){
        GraphTaskSecondaryThread* ptr = new GraphTaskSecondaryThread();
        ptr->SetGraphTaskThreadPoolInfo(&graph_task_queue_, &graph_task_priority_queue_, config_);
        status += ptr->Init();
        secondary_threads_.emplace_back(ptr);
    }
    return status;
}

RStatus GraphTaskThreadPool::ReleaseSecondaryThread(uint32_t size){
    std::lock_guard<std::mutex> lg(secondary_threads_lock_);
    for(auto iter = secondary_threads_.begin(); iter != secondary_threads_.end(); ++iter){
        GraphTaskSecondaryThread* ptr = *iter;
        if(!ptr->is_done_){
            secondary_threads_.erase(iter);
            delete ptr;
        }
    }

    ASSERT_NO_STRING(size <= secondary_threads_.size())

    for(auto iter = secondary_threads_.begin(); size != 0; --size, ++iter)
        (*iter)->is_done_ = false;
    
    return RStatus();
}

int32_t GraphTaskThreadPool::Dispatch(int32_t origin_index){
    int32_t real_index = 0;
    if(origin_index == kDefaultGraphTaskStrategy){
        real_index = cur_pool_id_++;
        if(cur_pool_id_ >= (int32_t)config_->max_thread_size_ || cur_pool_id_ < 0)
            cur_pool_id_ = 0;
    }else{
        real_index = origin_index;
    }
    return real_index;
}

GraphTaskThreadPool::MonitorRunnable::MonitorRunnable(GraphTaskThreadPool* pool): pool_(pool){}

RStatus GraphTaskThreadPool::MonitorRunnable::Run(){
    while(pool_->config_->monitor_enable_){
        while(pool_->config_->monitor_enable_ && !pool_->is_init_)
            std::this_thread::sleep_for(std::chrono::seconds(1));

        uint32_t span = pool_->config_->monitor_span_;
        while(pool_->config_->monitor_enable_ && pool_->is_init_ && span--)
            std::this_thread::sleep_for(std::chrono::seconds(1));

        bool is_busy = !pool_->primary_threads_.empty() && std::all_of(pool_->primary_threads_.begin(),
                pool_->primary_threads_.end(), [](GraphTaskPrimaryThread* ptr){return ptr != nullptr && ptr->is_running_; });

        std::lock_guard<std::mutex> lg(pool_->secondary_threads_lock_);
        if(is_busy || !pool_->graph_task_priority_queue_.IsEmpty())
            pool_->CreateSecondaryThread(1);

        for(auto iter = pool_->secondary_threads_.begin(); iter != pool_->secondary_threads_.end(); ++iter){
            GraphTaskSecondaryThread* ptr = *iter;
            if(ptr->Freeze()){
                pool_->secondary_threads_.erase(iter);
                delete ptr;
            }
        }
    }
    return RStatus();
}

RStatus GraphTaskThreadPool::Submit(const GraphTaskGroup& task_group, long ttl){
    ASSERT_NO_STRING(is_init_ == true)
    RStatus status;
    std::vector<std::future<void>> futures;
    for(const auto& task: task_group.task_vector_)
        futures.emplace_back(Commit(task));

    auto dead_line = std::chrono::steady_clock::now() +
            std::chrono::milliseconds(Min(task_group.GetTTL(), ttl));
    for(std::future<void>& future: futures){
        const auto& future_status = future.wait_until(dead_line);
        switch(future_status){
            case std::future_status::ready: 
                break;
            case std::future_status::timeout:
                status += RStatus("Thread Status TimeOut");
                break;
            case std::future_status::deferred:
                status += RStatus("Thread Status Deferred!");
                break;
            default:
                status += RStatus("Thread Status Unknown");
        }
    }

    if(task_group.on_finished_) task_group.on_finished_(status);
    return status;
}

RStatus GraphTaskThreadPool::Submit(const std::function<void()>& func, long ttl,
        const std::function<void(RStatus)>& on_finished){
    return Submit(GraphTaskGroup(func, ttl, on_finished));
}



}
