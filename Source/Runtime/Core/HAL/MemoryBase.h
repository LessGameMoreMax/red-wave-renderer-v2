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

    static MallocBase* CreateMallocBase();
    static void DeleteMallocBase();

    static void DebugInfo();

    static void* Malloc(std::size_t size, uint32_t alignment);
    static void Free(void* ptr);

    static void Memcpy(void* dest, const void* src, std::size_t count);
};

}
#endif
