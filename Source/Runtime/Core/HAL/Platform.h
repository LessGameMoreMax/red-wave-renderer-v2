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

class PlatformInfo{
private:
//Define PLATFORM TYPE
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WIN64_)
    constexpr static uint32_t kPlatformType = WINDOWS_PLATFORM_INDEX;
    #define PLATFORM_TYPE Windows
#elif defined(__linux__)
    constexpr static uint32_t kPlatformType = LINUX_PLATFORM_INDEX;
    #define PLATFORM_TYPE Linux
#else
#error UNKNOWN PLATFORM! SUPPORTED PLATFORM: WINDOWS, LINUX
#endif

//Define PLATFORM ARCH
#if defined(_WIN64) || defined(_WIN64_) || defined(__x86_64) || defined(__x86_64__)
    constexpr static uint32_t kPlatformArch = PLATFORM_ARCH_64;
#else
    constexpr static uint32_t kPlatformArch = PLATFORM_ARCH_32;
#endif
public:
    constexpr static bool IsWindows(){
        return kPlatformType == WINDOWS_PLATFORM_INDEX;
    }
    constexpr static bool IsLinux(){
        return kPlatformType == LINUX_PLATFORM_INDEX;
    }
    constexpr static bool IsArch32(){
        return kPlatformArch == PLATFORM_ARCH_32;
    }
    constexpr static bool IsArch64(){
        return kPlatformArch == PLATFORM_ARCH_64;
    }

    static void DebugInfo(){
        std::cout << MACRO_TO_STRING(PLATFORM_TYPE) << std::endl;
    }
};

#define INCLUDE_PLATFORM_HEADER(RELATIVE_PATH, NAME) \
    MACRO_TO_STRING(MACRO_LINK(RELATIVE_PATH/PLATFORM_TYPE/PLATFORM_TYPE, NAME))

}
#endif
