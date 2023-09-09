#ifndef PLATFORM_MEMORY_H
#define PLATFORM_MEMORY_H
#include "Platform.h"
#include INCLUDE_PLATFORM_HEADER(.., PlatformMemory.h)
namespace sablin{

template <typename T>
inline constexpr T Align(T address, uint32_t alignment){
    return (T)(((uint64_t)address + (uint64_t)alignment - 1) & ~((uint64_t)alignment - 1));
}

}
#endif
