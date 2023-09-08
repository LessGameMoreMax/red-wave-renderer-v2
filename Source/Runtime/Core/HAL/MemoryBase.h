#ifndef MEMORY_BASE_H
#define MEMORY_BASE_H
#include <cstdint>

namespace sablin{

class MallocBase;

class MemoryBase{
private:
    static MallocBase* kMalloc;
public:
    MemoryBase() = default;
    ~MemoryBase() = default;

    [[noreturn]] static void MemoryOverflow();
    static MallocBase* CreateMallocBase();
    static void DeleteMallocBase();

    static void DebugInfo();

    static void* Malloc(std::size_t size, uint32_t alignment);
    static void Free(void* ptr);
};

}
#endif
