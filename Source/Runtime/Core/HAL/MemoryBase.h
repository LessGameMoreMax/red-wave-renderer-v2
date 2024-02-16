#ifndef MEMORY_BASE_H
#define MEMORY_BASE_H
#include <cstdint>
#include "Platform.h"
#include INCLUDE_PLATFORM_HEADER(.., Platform.h)
#include "PlatformMemory.h"

namespace sablin{

class MallocBase;

class MemoryBase{
private:
    static MallocBase* kMalloc;
public:
    MemoryBase() = default;
    ~MemoryBase() = default;

    static MallocBase* CreateMallocBase();
    static void DeleteMallocBase();

    static void DebugInfo();

    static void* Malloc(std::size_t size, uint32_t alignment);
    static void* Realloc(void* ptr, std::size_t size, uint32_t alignment = MEMORY_ALIGNMENT_DEFAULT);
    static void Free(void* ptr);
    static std::size_t GetAllocSize(void* ptr);

    static void* BaseMalloc(std::size_t size);

    static void BaseFree(void* ptr);

    static inline void* SystemMalloc(std::size_t size){
        return ::malloc(size);
    }

    static inline void SystemFree(void* ptr){
        return ::free(ptr);
    }

    static inline void* Memcpy(void* dest, const void* src, std::size_t count){
        return PlatformMemory::Memcpy(dest, src, count);
    }

    static inline void* Memmove(void* dest, const void* src, std::size_t count){
        return PlatformMemory::Memmove(dest, src, count);
    }

    static inline int32_t Memcmp(const void* lhs, const void* rhs, std::size_t count){
        return PlatformMemory::Memcmp(lhs, rhs, count);
    }

    static inline void* Memset(void* dest, uint8_t character, std::size_t count){
        return PlatformMemory::Memset(dest, character, count);
    }

    static inline void* Memzero(void* dest, std::size_t count){
        return PlatformMemory::Memzero(dest, count);
    }
};

}
#endif
