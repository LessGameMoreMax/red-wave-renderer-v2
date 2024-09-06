#include "WorkThreadPool.h"
#include "Debug/Assertion.h"
#include "Core/GenericPlatform/GenericPlatformProcess.h"
#include "Math/MathTools.h"
#include "WorkThreadMonitorRunnable.h"
namespace sablin{

WorkThreadPool::WorkThreadPool():
    index_(0),
    timer_thread_(nullptr),
    timer_runnable_(nullptr),
    monitor_thread_(nullptr),
    has_init_(false),
    has_stop_(false){
    Initialize();
}

WorkThreadPool::~WorkThreadPool(){
    Exit();
}


void WorkThreadPool::Initialize(){
    uint32_t pool_size = CreateWorkPool(kWorkThreadMaxPoolSize);
    uint32_t thread_size = CreatePrimaryThreads(pool_size);
    ASSERT_WITH_STRING(thread_size != 0, "No Work Thread Was Setup!")
    while(thread_size != pool_size){
        delete work_pool_.back();
        work_pool_.pop_back();
    }
    CreateTimerThread();
    CreateMonitorThread();
    has_init_ = true;
}

void WorkThreadPool::Exit(){
    has_stop_ = true;
    ExitMonitorThread();
    ExitTimerThread();
    ExitPrimaryThreads();
    ExitWorkPool();
}

void WorkThreadPool::ExitMonitorThread(){
    monitor_thread_->StopRunnable();
    monitor_thread_->Join();
    Runnable* runnable = monitor_thread_->GetRunnable();
    delete monitor_thread_;
    delete runnable;
}

void WorkThreadPool::ExitTimerThread(){
    timer_thread_->StopRunnable();
    timer_thread_->Join();
    delete timer_thread_;
    delete timer_runnable_;
}

void WorkThreadPool::ExitPrimaryThreads(){
    for(uint32_t i = 0;i != GetPoolSize(); ++i){
        primary_work_threads_[i]->StopRunnable();
        work_pool_[i]->Stop();
        primary_work_threads_[i]->Join();
    }
    for(uint32_t i = 0;i != GetPoolSize(); ++i){
        delete primary_work_threads_[i];
        delete primary_runnable_pool_[i];
    }
}

void WorkThreadPool::ExitWorkPool(){
    for(uint32_t i = 0;i != GetPoolSize(); ++i)
        delete work_pool_[i];
}

uint32_t WorkThreadPool::CreateWorkPool(const uint32_t pool_size){
    for(uint32_t i = 0;i != pool_size; ++i){
        ProducerConsumerQueue<lenin::_WorkRunnable*>* temp =
            new ProducerConsumerQueue<lenin::_WorkRunnable*>();
        if(temp == nullptr) break;
        work_pool_.push_back(temp);
    }
    return work_pool_.size();
}

uint32_t WorkThreadPool::CreatePrimaryThreads(const uint32_t thread_num){
    for(uint32_t i = 0; i != thread_num; ++i){
        lenin::WorkThreadPrimaryRunnable* runnable = new lenin::WorkThreadPrimaryRunnable(this, i);
        if(runnable == nullptr) break;

        RunnableThread* runnable_thread = PlatformProcess::CreateNativeThread(runnable, "WorkThreadPrimaryThread",
                ThreadPriority::kThreadPriorityNormal, ThreadType::kThreadTypeWorkThreadPrimaryThread);
        if(runnable_thread == nullptr){
            delete runnable;
            break;
        }
        primary_work_threads_.push_back(runnable_thread);
        primary_runnable_pool_.push_back(runnable);
    }
    return primary_work_threads_.size();
}

void WorkThreadPool::CreateTimerThread(){
    timer_runnable_ = new lenin::WorkThreadTimerRunnable(this);
    ASSERT_NO_STRING(timer_runnable_ != nullptr)

    timer_thread_ = PlatformProcess::CreateNativeThread(timer_runnable_, "WorkThreadTimerThread",
            ThreadPriority::kThreadPriorityNormal, ThreadType::kThreadTypeWorkThreadTimerThread);
    ASSERT_NO_STRING(timer_thread_ != nullptr)
}

void WorkThreadPool::CreateMonitorThread(){
    lenin::WorkThreadMonitorRunnable* runnable = new lenin::WorkThreadMonitorRunnable(this);
    ASSERT_NO_STRING(runnable != nullptr)

    monitor_thread_ = PlatformProcess::CreateNativeThread(runnable, "WorkThreadMonitorThread",
            ThreadPriority::kThreadPriorityNormal, ThreadType::kThreadTypeWorkThreadMonitorThread);
    ASSERT_NO_STRING(monitor_thread_ != nullptr)
}

std::list<lenin::_WorkRunnable*> 
WorkThreadPool::StealWorks(uint32_t id, uint32_t max_num){
    std::list<lenin::_WorkRunnable*> result;
    if(!has_init_ || has_stop_) return result;
    size_t size = primary_runnable_pool_.size();
    for(uint32_t i = (id + 1) % size; i != id && max_num != 0; i = (i + 1) % size){
        lenin::WorkThreadPrimaryRunnable* runnable = primary_runnable_pool_[i];
        if(!runnable->TryLock()) continue;
        std::unique_lock<std::mutex> lk(runnable->GetLock(), std::adopt_lock);
        if(runnable->IsEmpty()) continue;
        result.push_back(runnable->Pop());
        --max_num;
    }
    return result;
}

lenin::_WorkRunnable* WorkThreadPool::PopFront(uint32_t id){
    return work_pool_[id]->PopFront();
}

std::list<lenin::_WorkRunnable*>
WorkThreadPool::PopFrontBatch(uint32_t id, uint32_t max_num){
    return work_pool_[id]->PopFrontBatch(max_num);
}

lenin::_WorkRunnable* WorkThreadPool::TryPopFront(uint32_t id){
    return work_pool_[id]->TryPopFront();
}

std::list<lenin::_WorkRunnable*>
WorkThreadPool::TryPopFrontBatch(uint32_t id, uint32_t max_num){
    return work_pool_[id]->TryPopFrontBatch(max_num);
}

void WorkThreadPool::PushBack(lenin::_WorkRunnable* runnable){
    if(has_stop_){
        delete runnable;
        return;
    }
    uint32_t index = index_++;
    index %= GetPoolSize();
    work_pool_[index]->PushBack(runnable);
}

void WorkThreadPool::PushTimerBack(lenin::WorkTimerRunnable* runnable){
    if(has_stop_ || runnable->work_timer_info_->HasStop()){
        delete runnable;
        return;
    }
    timer_runnable_->PushTimerBack(runnable);
}

}
