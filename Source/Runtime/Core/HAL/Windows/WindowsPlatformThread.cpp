#include "WindowsPlatformThread.h"
#if PLATFORM_INDEX == WINDOWS_PLATFORM_INDEX
#include <synchapi.h>
#include "Debug/Assertion.h"
#include "Macro/MacroDefine.h"
#include "Core/Memory/ThreadLocalMemory.h"
#include "Core/Thread/Thread.h"
namespace sablin{

WindowsPlatformThread::WindowsPlatformThread(Runnable* runnable, std::string thread_name,
        ThreadPriority thread_priority, ThreadType thread_type, uint32_t stack_size):
    RunnableThread(runnable, thread_name, thread_priority, thread_type, stack_size),
    thread_(0), wthread_param_(nullptr){}

WindowsPlatformThread::~WindowsPlatformThread(){
    Kill(true);
}

bool WindowsPlatformThread::SetThreadPriority(const ThreadPriority thread_priority){
    if(!HasSetup()) return;
    thread_priority_ = thread_priority;
    return ::SetThreadPriority(thread_, TranslatePriority(thread_priority_));
}

int WindowsPlatformThread::TranslatePriority(const ThreadPriority& thread_priority){
    ASSERT_WITH_STRING((uint32_t)ThreadPriority::Count == 7, "Please Change Translate Function At The Same Time!")
    switch((uint32_t)thread_priority){
        case (uint32_t)ThreadPriority::kThreadPriorityIdle:
            return THREAD_PRIORITY_IDLE;
        case (uint32_t)ThreadPriority::kThreadPriorityLowest:
            return THREAD_PRIORITY_LOWEST;
        case (uint32_t)ThreadPriority::kThreadPriorityBelowNormal:
            return THREAD_PRIORITY_BELOW_NORMAL;
        case (uint32_t)ThreadPriority::kThreadPriorityNormal:
            return THREAD_PRIORITY_NORMAL;
        case (uint32_t)ThreadPriority::kThreadPriorityAboveNormal:
            return THREAD_PRIORITY_ABOVE_NORMAL;
        case (uint32_t)ThreadPriority::kThreadPriorityHighest:
            return THREAD_PRIORITY_HIGHEST;
        case (uint32_t)ThreadPriority::kThreadPriorityTimeCritical:
            return THREAD_PRIORITY_TIME_CRITICAL;
        default:
#ifdef DEBUG
            ASSERT_WITH_STRING(false, "Wrong Thread Priority!")
#endif
            return THREAD_PRIORITY_NORMAL;
    }
}

bool WindowsPlatformThread::SetThreadAffinity(const CpuSet& cpu_set){
    if(!HasSetup()) return false;
    if(cpu_set.cpu_set_.empty()) return true;
    GROUP_AFFINITY group_affinity;
    group_affinity.Reserved[0] = 0;
    group_affinity.Reserved[1] = 0;
    group_affinity.Reserved[2] = 0;
    group_affinity.Group = GetAffinityGroupIndex(*cpu_set.cpu_set_.begin());
    for(uint16_t cpu_id: cpu_set.cpu_set_){
        ASSERT_WITH_STRING(GetAffinityGroupIndex(cpu_id) == group_affinity.Group, "Cores of binding must in the same group!")
        group_affinity.Mask |= THREAD_AFFINITY_MASK_1(cpu_id);
    }
    return SetThreadGroupAffinity(thread_, &group_affinity, nullptr);
}

WORD WindowsPlatformThread::GetAffinityGroupIndex(const uint16_t index){
    int16_t count = index;
    WORD group_count = GetActiveProcessorGroupCount();
    for(WORD i = 0;i != group_count; ++i){
        count -= GetActiveProcessorCount(i);
        if(count < 0) return i;
    }
    //Pay Attention: WORD is unsigned!
    return -1;
}

bool WindowsPlatformThread::SetupThread(const CpuSet& cpu_set){
    wthread_param_ = new WThreadParam{this, RStatus(STRING_DEFAULT)};
    lock_.lock();
    thread_ = CreateThread(NULL, stack_size_, &WindowsPlatformThread::_Run, 
            (LPVOID)wthread_param_, CREATE_SUSPENDED, NULL);
    if(thread_ == NULL){
        delete wthread_param_;
        wthread_param_ = nullptr;
    }else{
        joinable_ = true;
        ASSERT_NO_STRING(thread_id_ == -1)
        static uint32_t thread_id = 0;
        thread_id_ = thread_id++;
        SetThreadPriority(thread_priority_);
        SetThreadAffinity(cpu_set);
        ResumeThread(thread_);
    }
    lock_.unlock();
    return joinable_;
}

RStatus WindowsPlatformThread::Kill(bool should_wait){
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

    delete wthread_param_;
    wthread_param_ = nullptr;

    CloseHandle(thread_);
    return status;
}

void WindowsPlatformThread::Suspend(bool should_pause){
    if(should_pause){
        SuspendThread(thread_);
    }else{
        ResumeThread(thread_);
    }
}

RStatus WindowsPlatformThread::Join(){
    joinable_ = false;
    WaitForSingleObject(thread_, INFINITE);
    return wthread_param_->status_;
}

RStatus WindowsPlatformThread::Run(){
    RStatus status;
    status += runnable_->Init();
    if(status.IsOk()){
        status += runnable_->Run();
    }
    status += runnable_->Destroy();
    return status;
}

DWORD WINAPI WindowsPlatformThread::_Run(LPVOID wthread_param){
    WThreadParam* param = static_cast<WThreadParam*>(wthread_param);
    WindowsPlatformThread* thread = param->windows_platform_thread_;
    ThreadLocalMemory thread_local_memory;
    thread->lock_.lock();
    ThisThread::thread_id_ = thread->GetThreadId();
    thread->lock_.unlock();
    param->status_ = thread->Run();
    return 0;
}


}
#endif
