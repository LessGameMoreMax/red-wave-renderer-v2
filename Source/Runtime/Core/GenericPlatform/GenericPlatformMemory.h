#ifndef GENERIC_PLATFORM_MEMORY_H
#define GENERIC_PLATFORM_MEMORY_H
#include <cstdint>

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
    static void Memcpy(void* dest, const void* src, std::size_t count);
};

}
#endif
