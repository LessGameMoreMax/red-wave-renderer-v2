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
};

using PlatformMemory = WindowsPlatformMemory;
}
#endif
