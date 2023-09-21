#ifndef WINDOWS_PLATFORM_MEMORY_H
#define WINDOWS_PLATFORM_MEMORY_H
#include "../GenericPlatform/GenericPlatformMemory.h"

namespace sablin{

class WindowsPlatformMemory final: public GenericPlatformMemory{
private:
public:
    WindowsPlatformMemory() = default;
    virtual ~WindowsPlatformMemory() = default;

    static void Initialize();
    static MallocBase* CreateMallocBase();

    static void DebugInfo();
    [[noreturn]] static void MemoryOverflow();

    static std::pair<void*, std::size_t> BaseMalloc(std::size_t size, std::size_t alignment);

    static void BaseFree(void* ptr, std::size_t size);
};

using PlatformMemory = WindowsPlatformMemory;
}
#endif
