#ifndef WINDOWS_PLATFORM_THREAD_H
#define WINDOWS_PLATFORM_THREAD_H

#include "Macro/Platform.h"
#if PLATFORM_INDEX == WINDOWS_PLATFORM_INDEX
#include "Core/Thread/RunnableThread.h"
#include <windows.h>
#include <mutex>
namespace sablin{

class WindowsPlatformThread: public RunnableThread{
private:
    HANDLE thread_;

    struct WThreadParam{
        WindowsPlatformThread* windows_platform_thread_;
        RStatus status_;
    };

    WThreadParam* wthread_param_;
    std::mutex lock_;
private:
    int TranslatePriority(const ThreadPriority& thread_priority);
    WORD GetAffinityGroupIndex(const uint16_t index);
    static DWORD WINAPI _Run(LPVOID wthread_param);
public:
    explicit WindowsPlatformThread(Runnable* runnable, std::string thread_name,
            ThreadPriority thread_priority, ThreadType thread_type, uint32_t stack_size);
    virtual ~WindowsPlatformThread();

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
