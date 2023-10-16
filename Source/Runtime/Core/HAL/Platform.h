#ifndef PLATFORM_H
#define PLATFORM_H
#include "../Misc/MacroTools.h"
#include <assert.h>
#include <iostream>
namespace sablin{

#define WINDOWS_PLATFORM_INDEX 1
#define LINUX_PLATFORM_INDEX 2

#define PLATFORM_ARCH_32 1
#define PLATFORM_ARCH_64 2


//Define PLATFORM TYPE
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WIN64_)
    #define PLATFORM_INDEX WINDOWS_PLATFORM_INDEX
    #define PLATFORM_TYPE Windows
    #define THREAD_DEFAULT_STACK_SIZE 1024
#elif defined(__linux__) || defined(__linux) || defined(__gnu_linux__)
    #define PLATFORM_INDEX LINUX_PLATFORM_INDEX
    #define PLATFORM_TYPE Linux
    #define THREAD_DEFAULT_STACK_SIZE 8192
#else
#error UNKNOWN PLATFORM! SUPPORTED PLATFORM: WINDOWS, LINUX
#endif

//Define PLATFORM ARCH
#if defined(_WIN64) || defined(_WIN64_) || defined(__x86_64) || defined(__x86_64__)
    #define PLATFORM_ARCH PLATFORM_ARCH_64
#else
    #define PLATFORM_ARCH PLATFORM_ARCH_32
#endif

class PlatformInfo{
private:
public:
    inline static void DebugInfo(){
        std::cout << MACRO_TO_STRING(PLATFORM_TYPE) << std::endl;
    }
};

#define INCLUDE_PLATFORM_HEADER(RELATIVE_PATH, NAME) \
    MACRO_TO_STRING(MACRO_LINK(RELATIVE_PATH/PLATFORM_TYPE/PLATFORM_TYPE, NAME))


}
#endif
