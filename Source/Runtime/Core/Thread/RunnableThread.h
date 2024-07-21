#ifndef RUNNABLE_THREAD_H
#define RUNNABLE_THREAD_H
#include "Core/Thread/Runnable.h"
#include "Macro/MacroTools.h"
#include "Macro/Platform.h"
#include INCLUDE_PLATFORM_HEADER(PlatformAffinity.h)
namespace sablin{

class RunnableThread: public Runnable{
protected:
    std::string thread_name_;
    Runnable* runnable_;
    int32_t thread_id_;
    ThreadPriority thread_priority_;
    ThreadType thread_type_;
    uint32_t stack_size_;
    bool joinable_;
public:
    explicit RunnableThread(Runnable* runnable, std::string thread_name, 
            ThreadPriority thread_priority, ThreadType thread_type, uint32_t stack_size);
    virtual ~RunnableThread() = default;

    virtual bool SetThreadPriority(const ThreadPriority thread_priority) = 0;
    virtual bool SetThreadAffinity(const CpuSet& cpu_set) = 0;
    virtual bool SetupThread(const CpuSet& cpu_set) = 0;
    virtual RStatus Kill(bool should_wait) = 0;
    virtual void Suspend(bool should_pause) = 0;
    virtual RStatus Join() = 0;

    virtual void StopRunnable();

    constexpr bool IsJoinable() const{
        return joinable_;
    }

    constexpr int32_t GetThreadId() const{
        return thread_id_;
    }

    inline std::string GetThreadName() const{
        return thread_name_;
    }

    inline ThreadPriority GetThreadPriority() const{
        return thread_priority_;
    }

    inline ThreadType GetThreadType() const{
        return thread_type_;
    }

    inline bool HasSetup() const{
        return thread_id_ != -1;
    }

    inline Runnable* GetRunnable() const{
        return runnable_;
    }
};
}
#endif
