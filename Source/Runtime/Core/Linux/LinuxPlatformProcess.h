#ifndef LINUX_PLATFORM_PROCESS_H
#define LINUX_PLATFORM_PROCESS_H

#include "../HAL/Platform.h"
#if PLATFORM_INDEX == LINUX_PLATFORM_INDEX
#include "../TaskGraph/RunnableThread.h"

namespace sablin{

class LinuxPlatformProcess{
public:
    explicit LinuxPlatformProcess() = default;
    virtual ~LinuxPlatformProcess() = default;

    static RunnableThread* CreateThread(Runnable* runnable, std::string thread_name = STRING_DEFAULT,
            ThreadPriority thread_priority = ThreadPriority::kThreadPriorityNormal, 
            uint32_t stack_size = THREAD_DEFAULT_STACK_SIZE, CpuSet cpu_set = CpuSet());
};

using PlatformProcess = LinuxPlatformProcess;

}
#endif

#endif
