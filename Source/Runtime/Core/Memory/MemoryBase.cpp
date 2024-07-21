#include "MemoryBase.h"
#include "Debug/Assertion.h"
#include "Macro/MacroDefine.h"
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

void* MemoryBase::BaseMalloc(std::size_t size){
#ifdef DEBUG
    std::cout << "MemoryBase::BaseMalloc(std::size_t size)" << std::endl;
#endif
    if(size == 0) [[unlikely]]{
        size = 1;
    }
#if !defined(USE_PLATFORM_ALIGNMENT) && __STDCPP_DEFAULT_NEW_ALIGNMENT__ <= 8u
    return Malloc(size, MEMORY_MIN_ALIGNMENT);
#else
    return Malloc(size, MEMORY_ALIGNMENT_DEFAULT);
#endif
}

void MemoryBase::BaseFree(void* ptr){
#ifdef DEBUG
    std::cout << "MemoryBase::BaseFree(void* ptr)" << std::endl;
#endif
    Free(ptr);
}


}
