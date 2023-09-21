#ifndef WINDOWS_PLATFORM_MEMORY_H
#define WINDOWS_PLATFORM_MEMORY_H
#include "../GenericPlatform/GenericPlatformMemory.h"
#include <map>
#include <cstdint>

namespace sablin{

class WindowsPlatformMemory final: public GenericPlatformMemory{
private:
    struct RawPtrSize{
        void* raw_ptr_;
	std::size_t raw_size_;
    };
    static std::map<void*, RawPtrSize> ptr_map_;
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
