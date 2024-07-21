#include "WorkThreadPool.h"
#include "Debug/Assertion.h"
#include "Core/GenericPlatform/GenericPlatformProcess.h"
#include "WorkThreadRunnable.h"
#include "Math/MathTools.h"
namespace sablin{

WorkThreadPool::WorkThreadPool():
        work_thread_min_num_(0), work_thread_max_num_(0){
    Setup();
}

WorkThreadPool::~WorkThreadPool(){
    Release(GetWorkThreadNum());
}

void WorkThreadPool::Setup(){
    ASSERT_WITH_STRING(work_threads_.empty(), "When Setup, WorkThreadPool Is Not Empty!")
    const uint32_t work_thread_num = Add(kWorkThreadConfigMinNum);
    ASSERT_WITH_STRING(work_thread_num != 0, "No Work Thread Was Setup!")
    work_thread_min_num_ = work_thread_num;
    work_thread_max_num_ = kWorkThreadConfigMaxNum;
}

void WorkThreadPool::Release(const int32_t thread_num){
    std::list<RunnableThread*> buffer;
    lock_.lock();
    for(int32_t i = 0;i != thread_num; ++i){
        RunnableThread* thread = work_threads_.back();
        thread->StopRunnable();
        buffer.push_front(thread);
        work_threads_.pop_back();
    }
    lock_.unlock();

    while(!buffer.empty()){
        Runnable* runnable = buffer.back()->GetRunnable();
        delete buffer.back();
        buffer.pop_back();
        delete runnable;
    }
}

uint32_t WorkThreadPool::Add(const int32_t thread_num){
    std::list<RunnableThread*> buffer;
    for(int32_t i = 0; i != thread_num; ++i){
        Runnable* runnable = new WorkThreadRunnable();
        if(runnable == nullptr){
            std::cout << "Work Thread Runnable Create Failed!" << std::endl;
            break;
        }

        RunnableThread* runnable_thread = PlatformProcess::CreateNativeThread(runnable, "WorkThread",
                ThreadPriority::kThreadPriorityNormal,
                ThreadType::kThreadTypeWorkThread);
        if(runnable_thread == nullptr){
            std::cout << "Work Thread Add Failed!" << std::endl;
            delete runnable;
            break;
        }
        buffer.push_back(runnable_thread);
    }
    lock_.lock();
    for(RunnableThread* runnable_thread: buffer) 
        work_threads_.push_back(runnable_thread);
    lock_.unlock();
    return buffer.size();
}

uint32_t WorkThreadPool::EnlargeWorkThread(const int32_t thread_num){
    if(thread_num <= 0) return 0;
    uint32_t left = kWorkThreadConfigMaxNum - GetWorkThreadNum();
    if(left == 0) return 0;

    return Add(Min<uint32_t>(thread_num, left));
}

bool WorkThreadPool::ReduceWorkThread(const int32_t thread_num){
    if(thread_num <= 0) return false;
    uint32_t left = GetWorkThreadNum() - work_thread_min_num_;
    if(left == 0) return false;
    Release(Min<uint32_t>(thread_num, left));
    return true;
}





}
