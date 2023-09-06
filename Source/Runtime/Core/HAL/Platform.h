#ifndef PLATFORM_H
#define PLATFORM_H
#include <assert.h>
#include <iostream>
namespace sablin{

// //Initialize Macro:
// #define PLATFORM_WINDOWS 0
// #define PLATFORM_LINUX 0
// #define PLATFORM_64 0
// #define PLATFORM_32 0
//
// //Define PLATFORM TYPE
// #if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WIN64_)
// #define PLATFORM_WINDOWS 1
// #elif defined(__linux__)
// #define PLATFORM_LINUX 1
// #else
// #error UNKNOWN PLATFORM! SUPPORTED PLATFORM: WINDOWS, LINUX
// #endif
//
// //Define PLATFORM ARCH
// #if defined(_WIN64) || defined(_WIN64_) || defined(__x86_64) || defined(__x86_64__)
// #define PLATFORM_64 1
// #else
// #define PLATFORM_32 1
// #endif

class Assert{
public:
    [[noreturn]] static void NoImplementAssert(const std::string& classname, const std::string& function_name){
        std::cout << "NoImplementAssert: " << classname << "::" << function_name << " Doesn't Have Any Implement!" << std::endl;
        assert(false);
    }
};

#define WINDOWS_PLATFORM_INDEX 1
#define LINUX_PLATFORM_INDEX 2

#define PLATFORM_ARCH_32 1
#define PLATFORM_ARCH_64 2

class PlatformInfo{
private:
//Define PLATFORM TYPE
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WIN64_)
    constexpr static uint32_t kPlatformType = WINDOWS_PLATFORM_INDEX;
#elif defined(__linux__)
    constexpr static uint32_t kPlatformType = LINUX_PLATFORM_INDEX;
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
};

}

#endif
