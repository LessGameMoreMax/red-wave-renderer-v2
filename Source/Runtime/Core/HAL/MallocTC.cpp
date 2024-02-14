#include "MallocTC.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
#include "TCGlobals.h"
#include "PlatformMemory.h"
#include "MemoryBase.h"
#include "../../Math/MathTools.h"
#include "TCCommon.h"
#include "TCThreadCache.h"
#include "TCPageCache.h"

#include <thread>
namespace sablin{

MallocTC::MallocTC():
        MallocBase()
{
    TCGlobals::Initialize();
}

MallocTC::~MallocTC(){
    TCGlobals::Exit();
}

void* MallocTC::TCMalloc(std::size_t size, uint32_t alignment){
    size = RoundUp<std::size_t>(size, alignment);
    if(size > kMaxSize){
        return TCGlobals::page_cache_.AllocBig(size);
    }else{
        return TCGlobals::thread_local_cache_->Allocate(size);
    }
}

void MallocTC::TCFree(void* ptr){
    TCSpan* span = TCGlobals::page_cache_.MapObjectToSpan(ptr);
    std::size_t size = span->GetObjectSize();
    if(size > kMaxSize){
        TCGlobals::page_cache_.FreeBig(span);
    }else{
        TCGlobals::thread_local_cache_->Deallocate(ptr, size);
    }
}

void* MallocTC::Malloc(std::size_t size, uint32_t alignment){
    void* ptr = TryMalloc(size, alignment);
    if(ptr == nullptr)[[unlikely]] PlatformMemory::MemoryOverflow();
    return ptr;
}

void* MallocTC::TryMalloc(std::size_t size, uint32_t alignment){
    if(size < MEMORY_FORCE_MIN_ALIGNMENT_SIZE){
        alignment = MEMORY_MIN_ALIGNMENT;
    }else{
        alignment = Max<uint32_t>(MEMORY_ALIGNMENT_DEFAULT, alignment);
    }
    return TCMalloc(size, alignment);
}

void* MallocTC::Realloc(void* ptr, std::size_t new_size, uint32_t alignment){
    void* result = TryRealloc(ptr, new_size, alignment);
    if(result == nullptr && new_size != 0)[[unlikely]] PlatformMemory::MemoryOverflow();
    return result;
}

void* MallocTC::TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment){
    if(new_size < MEMORY_FORCE_MIN_ALIGNMENT_SIZE){
        alignment = MEMORY_MIN_ALIGNMENT;
    }else{
        alignment = Max<uint32_t>(MEMORY_ALIGNMENT_DEFAULT, alignment);
    }
    if(ptr != nullptr)[[likely]]{
        if(new_size != 0){
            void* new_ptr = TCMalloc(new_size, alignment);
            if(new_ptr == nullptr) return nullptr;
            std::size_t old_size = GetAllocatedSize(ptr);
            MemoryBase::Memcpy(new_ptr, ptr, Min<std::size_t>(old_size, new_size));
            TCFree(ptr);
            return new_ptr;
        }else{
            TCFree(ptr);
            return nullptr;
        }
    }
    if(new_size != 0) return TCMalloc(new_size, alignment);
    return nullptr;
}

void MallocTC::Free(void* ptr){
    TCFree(ptr);
}

std::string MallocTC::GetDescriptName() const{
    return "TC";
}

bool MallocTC::IsThreadSafe() const{
    return true;
}

std::size_t MallocTC::GetAllocatedSize(void* ptr){
    return TCGlobals::page_cache_.MapObjectToSpan(ptr)->GetObjectSize();
}

}
