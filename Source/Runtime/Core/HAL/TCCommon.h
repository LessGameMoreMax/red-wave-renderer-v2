#ifndef TC_COMMON_H
#define TC_COMMON_H
#include <cstdint>
#include "../Misc/MacroDefine.h"
#include "Platform.h"
namespace sablin{
#ifndef TC_MALLOC_PAGE_SHIFT
#ifdef TC_MALLOC_32K_PAGES
    #define TC_MALLOC_PAGE_SHIFT 15
#elif defined(TC_MALLOC_256K_PAGES)
    #define TC_MALLOC_PAGE_SHIFT 18
#else
    #error "NO DEFAULT TC_MALLOC_SIZE_PAGES"
#endif
#endif

#if TC_MALLOC_PAGE_SHIFT == 15
    inline constexpr std::size_t kPageShift = 15;
    inline constexpr std::size_t kMaxSize = 256 * 1024;
    // inline constexpr std::size_t kMinThreadCacheSize = 512 * 1024;
    // inline constexpr std::size_t kMaxThreadCacheSize = 4096 * 1024;
    // inline constexpr std::size_t kMaxCpuCacheSize = 1.5f * 1024 * 1024;
    // inline constexpr std::size_t kOverThreadCacheSize = 8u * kMaxThreadCacheSize;
    // inline constexpr std::size_t kStealSize = 64 * 1024;
#elif TC_MALLOC_PAGE_SHIFT == 18
    inline constexpr std::size_t kPageShift = 18;
    inline constexpr std::size_t kMaxSize = 256 * 1024;
    // inline constexpr std::size_t kMinThreadCacheSize = 512 * 1024;
    // inline constexpr std::size_t kMaxThreadCacheSize = 4096 * 1024;
    // inline constexpr std::size_t kMaxCpuCacheSize = 1.5f * 1024 * 1024;
    // inline constexpr std::size_t kOverThreadCacheSize = 8u * kMaxThreadCacheSize;
    // inline constexpr std::size_t kStealSize = 64 * 1024;
#else
    #error "NO DEFAULT TC_MALLOC_PAGE_SHIFT INITIALIZE"
#endif

    inline constexpr std::size_t kPageSize = 1 << kPageShift;
    inline constexpr std::size_t kMaxOverages = 3;
    inline constexpr std::size_t kMaxDynamicFreeListLength = 8192;
    inline constexpr std::size_t kMinMoveNum = 2;
    inline constexpr std::size_t kMaxMoveNum = 128;

#if PLATFORM_ARCH == PLATFORM_ARCH_32
    inline constexpr uint32_t kAddressBits = 8 * sizeof(void*);
#else
    inline constexpr uint32_t kAddressBits = 48;
#endif
}
#endif
