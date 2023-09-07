#ifndef LINUX_PLATFORM_MEMORY_H
#define LINUX_PLATFORM_MEMORY_H
#include "../GenericPlatform/GenericPlatformMemory.h"

namespace sablin{

class LinuxPlatformMemory final: public GenericPlatformMemory{
private:
public:
    LinuxPlatformMemory() = default;
    virtual ~LinuxPlatformMemory() = default;

    static void Initialize();
    static MallocBase* CreateMallocBase();

    static void DebugInfo();
};

using PlatformMemory = LinuxPlatformMemory;

}
#endif
