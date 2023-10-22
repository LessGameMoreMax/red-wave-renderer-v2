#ifndef LINUX_PLATFORM_THREAD_H
#define LINUX_PLATFORM_THREAD_H

#include "../HAL/Platform.h"
#if PLATFORM_INDEX == LINUX_PLATFORM_INDEX
#include "../HAL/RunnableThread.h"
#include <pthread.h>
#include <mutex>
namespace sablin{

class LinuxPlatformThread: public RunnableThread{
private:
    pthread_t thread_;

    struct PThreadParam{
        LinuxPlatformThread* linux_platform_thread_;
        RStatus status_;
    };

    PThreadParam* pthread_param_;
    std::mutex lock_;
private:
    sched_param TranslatePriority(const ThreadPriority& thread_priority);
    static void* _Run(void* pthread_param);
public:
    explicit LinuxPlatformThread(Runnable* runnable, std::string thread_name,
            ThreadPriority thread_priority, ThreadType thread_type, uint32_t stack_size);
    virtual ~LinuxPlatformThread();

    virtual bool SetThreadPriority(const ThreadPriority thread_priority) override;
    virtual bool SetThreadAffinity(const CpuSet& cpu_set) override;
    virtual bool SetupThread(const CpuSet& cpu_set) override;
    virtual RStatus Kill(bool should_wait) override;
    virtual void Suspend(bool should_pause) override;
    virtual RStatus Join() override;

    virtual RStatus Run() override;
};

}
#endif

#endif
