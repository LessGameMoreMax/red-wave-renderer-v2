#ifndef WINDOWS_PLATFORM_PROCESS_H
#define WINDOWS_PLATFORM_PROCESS_H

#include "../HAL/Platform.h"
#if PLATFORM_INDEX == WINDOWS_PLATFORM_INDEX
#include "../HAL/RunnableThread.h"

namespace sablin{

class WindowsPlatformProcess{
public:
    explicit WindowsPlatformProcess() = default;
    virtual ~WindowsPlatformProcess() = default;

    static RunnableThread* CreateNativeThread(Runnable* runnable, std::string thread_name = STRING_DEFAULT,
            ThreadPriority thread_priority = ThreadPriority::kThreadPriorityNormal, ThreadType thread_type = ThreadType::kThreadTypeNative,
            uint32_t stack_size = THREAD_DEFAULT_STACK_SIZE, CpuSet cpu_set = CpuSet());
};

using PlatformProcess = WindowsPlatformProcess;

}
#endif

#endif
