#include "MemoryBase.h"
#include "../Debug/Assertion.h"
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

void* MemoryBase::Realloc(void* ptr, std::size_t size, uint32_t alignment){
    return kMalloc->Realloc(ptr, size, alignment);
}

void MemoryBase::Free(void* ptr){
    kMalloc->Free(ptr);
}

std::size_t MemoryBase::GetAllocSize(void* ptr){
    return kMalloc->GetAllocatedSize(ptr);
}


}
