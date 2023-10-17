#include "LinuxPlatformProcess.h"

#if PLATFORM_INDEX == LINUX_PLATFORM_INDEX
#include "../Math/MathTools.h"
#include "../Linux/LinuxPlatformThread.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include "../Debug/Assertion.h"
#endif
namespace sablin{

RunnableThread* LinuxPlatformProcess::CreateNativeThread(Runnable* runnable, std::string thread_name, 
        ThreadPriority thread_priority, ThreadType thread_type, uint32_t stack_size, CpuSet cpu_set){
    stack_size = Max<uint32_t>(stack_size, THREAD_DEFAULT_STACK_SIZE);
    RunnableThread* runnable_thread = new LinuxPlatformThread(runnable, thread_name, thread_priority, thread_type, stack_size);

    if(!runnable_thread->SetupThread(cpu_set)){
        std::cout << "Warning: Fail to setup thread!" << std::endl;
#ifdef DEBUG
        ASSERT_NO_STRING(false)
#endif
    }
    return runnable_thread;
}

}
#endif
