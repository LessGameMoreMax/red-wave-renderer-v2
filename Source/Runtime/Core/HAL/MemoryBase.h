#ifndef MEMORY_BASE_H
#define MEMORY_BASE_H

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

    static void DebugInfo();
    
};

}
#endif
