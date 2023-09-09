#include "MemoryBase.h"
#include "../Debug/Assertion.h"
#include "PlatformMemory.h"
#include "../Misc/MacroDefine.h"
#include "MallocBase.h"
namespace sablin{

MallocBase* MemoryBase::kMalloc = nullptr;


MallocBase* MemoryBase::CreateMallocBase(){
#ifdef DEBUG
    ASSERT_WITH_STRING(kMalloc == nullptr, "Repeat CreateMallocBase!")
#endif
    kMalloc = PlatformMemory::CreateMallocBase();
    return kMalloc;
}

void MemoryBase::DeleteMallocBase(){
    delete kMalloc;
    kMalloc = nullptr;
}

void MemoryBase::DebugInfo(){
    PlatformMemory::DebugInfo();
}

void* MemoryBase::Malloc(std::size_t size, uint32_t alignment){
    return kMalloc->Malloc(size, alignment);
}

void MemoryBase::Free(void* ptr){
    kMalloc->Free(ptr);
}

void MemoryBase::Memcpy(void* dest, const void* src, std::size_t count){
    PlatformMemory::Memcpy(dest, src, count);
}

}
