#include "LinuxPlatformThread.h"
#if PLATFORM_INDEX == LINUX_PLATFORM_INDEX
#include "Debug/Assertion.h"
#include "Macro/MacroDefine.h"
#include "Core/Memory/ThreadLocalMemory.h"
#include "Core/Thread/Thread.h"
namespace sablin{

LinuxPlatformThread::LinuxPlatformThread(Runnable* runnable, std::string thread_name,
        ThreadPriority thread_priority, ThreadType thread_type, uint32_t stack_size): 
    RunnableThread(runnable, thread_name, thread_priority, thread_type, stack_size), 
    thread_(0), pthread_param_(nullptr) {}

LinuxPlatformThread::~LinuxPlatformThread(){
    Kill(true);
}

bool LinuxPlatformThread::SetThreadPriority(const ThreadPriority thread_priority){
    thread_priority_ = thread_priority;
    sched_param param = TranslatePriority(thread_priority_);
    return pthread_setschedparam(thread_, SCHED_RR, &param) == 0;
}

sched_param LinuxPlatformThread::TranslatePriority(const ThreadPriority& thread_priority){
    ASSERT_WITH_STRING((uint32_t)ThreadPriority::Count == 7, "Please Change Translate Function At The Same Time!")

    switch((uint32_t)thread_priority){
        case (uint32_t)ThreadPriority::kThreadPriorityIdle:
            return sched_param{10};
        case (uint32_t)ThreadPriority::kThreadPriorityLowest:
            return sched_param{25};
        case (uint32_t)ThreadPriority::kThreadPriorityBelowNormal:
            return sched_param{40};
        case (uint32_t)ThreadPriority::kThreadPriorityNormal:
            return sched_param{50};
        case (uint32_t)ThreadPriority::kThreadPriorityAboveNormal:
            return sched_param{60};
        case (uint32_t)ThreadPriority::kThreadPriorityHighest:
            return sched_param{75};
        case (uint32_t)ThreadPriority::kThreadPriorityTimeCritical:
            return sched_param{90};
        default:
#ifdef DEBUG
            ASSERT_WITH_STRING(false, "Wrong Thread Priority!")
#endif
            return sched_param{50};
    }
}

bool LinuxPlatformThread::SetThreadAffinity(const CpuSet& cpu_set){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    for(uint16_t cpu_id: cpu_set.cpu_set_){
        CPU_SET(cpu_id, &mask);
    }
    return pthread_setaffinity_np(thread_, sizeof(cpu_set_t), &mask) == 0;
}

bool LinuxPlatformThread::SetupThread(const CpuSet& cpu_set){
    pthread_attr_t attr;
    ASSERT_NO_STRING(pthread_attr_init(&attr) == 0)
    //Set Detach State
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    //Set Sched Policy
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    //Set Thread Priority
    sched_param param = TranslatePriority(thread_priority_);
    pthread_attr_setschedparam(&attr, &param);
    //Set Stack Size
    pthread_attr_setstacksize(&attr, stack_size_);

    ASSERT_NO_STRING(pthread_param_ == nullptr)
    pthread_param_ = new PThreadParam{this, RStatus(STRING_DEFAULT)};
    
    lock_.lock();
    if(pthread_create(&thread_, &attr, &LinuxPlatformThread::_Run, (void*)pthread_param_) == 0){
        SetThreadAffinity(cpu_set);
        joinable_ = true;
        ASSERT_NO_STRING(thread_id_ == -1)
        static uint32_t thread_id = 0;
        thread_id_ = thread_id++;
    }else{
        delete pthread_param_;
        pthread_param_ = nullptr;
    }
    lock_.unlock();
    ASSERT_NO_STRING(pthread_attr_destroy(&attr) == 0)
    return joinable_;
}

RStatus LinuxPlatformThread::Kill(bool should_wait){
    RStatus status;
    if(!HasSetup()) return status; 
    thread_id_ = -1;

    if(runnable_ != nullptr){
        runnable_->Stop();
    }

    if(should_wait && joinable_){
        status += Join();
    }
    joinable_ = false;

    delete pthread_param_;
    pthread_param_ = nullptr;
    return status;
}

void LinuxPlatformThread::Suspend(bool should_pause){
}

RStatus LinuxPlatformThread::Join(){
    joinable_ = false;
    pthread_join(thread_, NULL);
    return pthread_param_->status_;
}

RStatus LinuxPlatformThread::Run(){
    RStatus status;
    status += runnable_->Init();
    if(status.IsOk()){
        status += runnable_->Run();
    }
    status += runnable_->Destroy();
    return status;
}

void* LinuxPlatformThread::_Run(void* pthread_param){
    PThreadParam* param = static_cast<PThreadParam*>(pthread_param);
    LinuxPlatformThread* thread = param->linux_platform_thread_;
    ThreadLocalMemory thread_local_memory;
    thread->lock_.lock();
    ThisThread::thread_id_ = thread->GetThreadId();
    thread->lock_.unlock();
    param->status_ = thread->Run();
    return nullptr;
}

}
#endif
