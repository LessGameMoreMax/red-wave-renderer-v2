#ifndef TC_COMMON_H
#define TC_COMMON_H
#include <cstdint>
#include "../Misc/MacroDefine.h"
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
    inline constexpr std::size_t kMinThreadCacheSize = 512 * 1024;
    inline constexpr std::size_t kMaxThreadCacheSize = 4096 * 1024;
    // inline constexpr std::size_t kMaxCpuCacheSize = 1.5f * 1024 * 1024;
    inline constexpr std::size_t kOverThreadcacheSize = 8u * kMaxThreadCacheSize;
#elif TC_MALLOC_PAGE_SHIFT == 18
    inline constexpr std::size_t kPageShift = 18;
    inline constexpr std::size_t kMaxSize = 256 * 1024;
    inline constexpr std::size_t kMinThreadCacheSize = 512 * 1024;
    inline constexpr std::size_t kMaxThreadCacheSize = 4096 * 1024;
    // inline constexpr std::size_t kMaxCpuCacheSize = 1.5f * 1024 * 1024;
    inline constexpr std::size_t kOverThreadcacheSize = 8u * kMaxThreadCacheSize;
#else
    #error "NO DEFAULT TC_MALLOC_PAGE_SHIFT INITIALIZE"
#endif

    inline constexpr std::size_t kPageSize = 1 << kPageShift;
    inline constexpr std::size_t kMaxDynamicFreeListLength = 8192;
}
#endif
