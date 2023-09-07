#include "MemoryBase.h"
#include "../Debug/Assertion.h"
#include "PlatformMemory.h"
namespace sablin{

MallocBase* MemoryBase::kMalloc = nullptr;

void MemoryBase::MemoryOverflow(){
    throw;
}

MallocBase* MemoryBase::CreateMallocBase(){
#ifdef DEBUG
    ASSERT_WITH_STRING(kMalloc == nullptr, "Repeat CreateMallocBase!")
#endif
    kMalloc = PlatformMemory::CreateMallocBase();
    return kMalloc;
}

void MemoryBase::DebugInfo(){
    PlatformMemory::DebugInfo();
}

}
