#ifndef PLATFORM_MEMORY_H
#define PLATFORM_MEMORY_H
#include "Platform.h"
#include INCLUDE_PLATFORM_HEADER(.., PlatformMemory.h)
namespace sablin{

template <typename T>
inline constexpr T Align(T address, uint32_t alignment){
    return (T)(((uint64_t)address + (uint64_t)alignment - 1) & ~((uint64_t)alignment - 1));
}

template <typename T>
inline constexpr T RoundDown(T size, T alignment){
    return (size) & ~(alignment-1);
}

template <typename T>
inline constexpr T RoundUp(T size, T alignment){
    return RoundDown<T>(size + alignment - 1, alignment);
}

}
#endif
