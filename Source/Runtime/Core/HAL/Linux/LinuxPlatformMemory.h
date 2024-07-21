#ifndef LINUX_PLATFORM_MEMORY_H
#define LINUX_PLATFORM_MEMORY_H

#include "Macro/Platform.h"
#if PLATFORM_INDEX == LINUX_PLATFORM_INDEX
#include "Core/GenericPlatform/GenericPlatformMemory.h"

namespace sablin{

class LinuxPlatformMemory final: public GenericPlatformMemory{
private:
public:
    LinuxPlatformMemory() = default;
    virtual ~LinuxPlatformMemory() = default;

    static void Initialize();
    static MallocBase* CreateMallocBase();

    static void DebugInfo();
    [[noreturn]] static void MemoryOverflow();

    static std::pair<void*, std::size_t> BaseMalloc(std::size_t size, std::size_t alignment);

    static void BaseFree(void* ptr, std::size_t size);
};

using PlatformMemory = LinuxPlatformMemory;

}
#endif

#endif
