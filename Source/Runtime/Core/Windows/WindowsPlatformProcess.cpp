#include "WindowsPlatformProcess.h"

#if PLATFORM_INDEX == WINDOWS_PLATFORM_INDEX
#include "../Math/MathTools.h"
#include "../Windows/WindowsPlatformThread.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include "../Debug/Assertion.h"
#endif
namespace sablin{

RunnableThread* WindowsPlatformProcess::CreateThread(Runnable* runnable, std::string thread_name,
        ThreadPriority thread_priority, uint32_t stack_size, CpuSet cpu_set){
    stack_size = Max<uint32_t>(stack_size, THREAD_DEFAULT_STACK_SIZE);
    RunnableThread* runnable_thread = new WindowsPlatformThread(runnable, thread_name, thread_priority, stack_size);
    
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
