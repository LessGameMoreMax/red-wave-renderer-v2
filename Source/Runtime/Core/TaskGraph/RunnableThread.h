#ifndef RUNNABLE_THREAD_H
#define RUNNABLE_THREAD_H
#include "Runnable.h"
#include "../Misc/MacroTools.h"
#include "../HAL/Platform.h"
#include INCLUDE_PLATFORM_HEADER(.., PlatformAffinity.h)
namespace sablin{

class RunnableThread: public Runnable{
protected:
    std::string thread_name_;
    Runnable* runnable_;
    uint32_t thread_id_;
    ThreadPriority thread_priority_;
    uint32_t stack_size_;
    bool joinable_;
public:
    explicit RunnableThread(Runnable* runnable, std::string thread_name, 
            ThreadPriority thread_priority, uint32_t stack_size);
    virtual ~RunnableThread() = default;

    virtual void SetThreadPriority(const ThreadPriority thread_priority) = 0;
    virtual bool SetThreadAffinity(const CpuSet& cpu_set) = 0;
    virtual bool SetupThread(const CpuSet& cpu_set = CpuSet()) = 0;
    virtual RStatus Kill(bool should_wait) = 0;
    virtual void Suspend(bool should_pause) = 0;
    virtual RStatus Join() = 0;

    constexpr bool IsJoinable() const{
        return joinable_;
    }

    constexpr uint32_t GetThreadId() const{
        return thread_id_;
    }

    inline std::string GetThreadName() const{
        return thread_name_;
    }

    inline ThreadPriority GetThreadPriority() const{
        return thread_priority_;
    }

    inline bool HasSetup() const{
        return thread_id_ != -1;
    }
};
}
#endif
