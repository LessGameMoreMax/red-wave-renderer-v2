#ifndef GENERIC_PLATFORM_MEMORY_H
#define GENERIC_PLATFORM_MEMORY_H
#include <cstdint>
#include <cstring>
#include "../Debug/Assertion.h"

namespace sablin{

class MallocBase;

class GenericPlatformMemory{
private:
public:
    GenericPlatformMemory() = default;
    virtual ~GenericPlatformMemory() = default;
    
    static void Initialize();
    static MallocBase* CreateMallocBase();

    static void DebugInfo();

    [[noreturn]] static void MemoryOverflow();

    static inline void* Memcpy(void* dest, const void* src, std::size_t count){
        return memcpy(dest, src, count);
    }

    static inline void* Memmove(void* dest, const void* src, std::size_t count){
        return memmove(dest, src, count);
    }

    static inline int32_t Memcmp(const void* lhs, const void* rhs, std::size_t count){
        return memcmp(lhs, rhs, count);
    }

    static inline void* Memset(void* dest, uint8_t character, std::size_t count){
        return memset(dest, character, count);
    }

    static inline void* Memzero(void* dest, std::size_t count){
        return memset(dest, 0, count);
    }

    static std::pair<void*, std::size_t> BaseMalloc(std::size_t size, std::size_t alignment){
        NO_IMPLEMENT_ASSERT(GenericPlatformMemory, BaseMalloc)
	return {nullptr, 0};
    }

    static void BaseFree(void* ptr, std::size_t size){
        NO_IMPLEMENT_ASSERT(GenericPlatformMemory, BaseFree)
    }
};

template <typename T>
inline constexpr T Align(T address, uint32_t alignment){
    return (T)(((uint64_t)address + (uint64_t)alignment - 1) & ~((uint64_t)alignment - 1));
}

}
#endif
