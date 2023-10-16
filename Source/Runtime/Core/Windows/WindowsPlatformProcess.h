#ifndef WINDOWS_PLATFORM_PROCESS_H
#define WINDOWS_PLATFORM_PROCESS_H

#include "../HAL/Platform.h"
#if PLATFORM_INDEX == WINDOWS_PLATFORM_INDEX
#include "../TaskGraph/RunnableThread.h"

namespace sablin{

class WindowsPlatformProcess{
public:
    explicit WindowsPlatformProcess() = default;
    virtual ~WindowsPlatformProcess() = default;

    static RunnableThread* CreateThread(Runnable* runnable, std::string thread_name = STRING_DEFAULT,
            ThreadPriority thread_priority = ThreadPriority::kThreadPriorityNormal, uint32_t stack_size = THREAD_DEFAULT_STACK_SIZE);
};

using PlatformProcess = WindowsPlatformProcess;

}
#endif

#endif
