#include "MallocAnsi.h"
#include "Macro/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
#include "PlatformMemory.h"
#include "Math/MathTools.h"
#include "MemoryBase.h"
namespace sablin{

MallocAnsi::MallocAnsi():
        MallocBase()
{

}

MallocAnsi::~MallocAnsi(){

}

void* MallocAnsi::AnsiMalloc(std::size_t size, uint32_t alignment){
    void* ptr = ::malloc(sizeof(std::size_t) + sizeof(void*) + alignment + size);
    if(ptr == nullptr) return nullptr;
    void* result = Align((uint8_t*)ptr + sizeof(std::size_t) + sizeof(void*), alignment);
    *(void**)((uint8_t*)result - sizeof(void*)) = ptr;
    *(std::size_t*)((uint8_t*)result - sizeof(void*) - sizeof(std::size_t)) = size;
    return result;
}

void MallocAnsi::AnsiFree(void* ptr){
    if(ptr == nullptr)[[unlikely]] return;
    ::free(*(void**)((uint8_t*)ptr - sizeof(void*))); 
}

void* MallocAnsi::Malloc(std::size_t size, uint32_t alignment){
#ifdef DEBUG
    std::cout << "MallocAnsi::Malloc" << std::endl;
#endif
    void* ptr = TryMalloc(size, alignment);
    if(ptr == nullptr)[[unlikely]] PlatformMemory::MemoryOverflow();
    return ptr;
}

void* MallocAnsi::TryMalloc(std::size_t size, uint32_t alignment){
    if(size < MEMORY_FORCE_MIN_ALIGNMENT_SIZE){
        alignment = MEMORY_MIN_ALIGNMENT;
    }else{
        alignment = Max<uint32_t>(MEMORY_ALIGNMENT_DEFAULT, alignment);
    }
    return AnsiMalloc(size, alignment);
}

void* MallocAnsi::Realloc(void* ptr, std::size_t new_size, uint32_t alignment){
#ifdef DEBUG
    std::cout << "MallocAnsi::Realloc" << std::endl;
#endif
    void* result = TryRealloc(ptr, new_size, alignment);
    if(result == nullptr && new_size != 0)[[unlikely]] PlatformMemory::MemoryOverflow();
    return result;
}

void* MallocAnsi::TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment){
    if(new_size < MEMORY_FORCE_MIN_ALIGNMENT_SIZE){
        alignment = MEMORY_MIN_ALIGNMENT;
    }else{
        alignment = Max<uint32_t>(MEMORY_ALIGNMENT_DEFAULT, alignment);
    }
    if(ptr != nullptr)[[likely]]{
        if(new_size != 0){
            void* new_ptr = AnsiMalloc(new_size, alignment);
            if(new_ptr == nullptr) return nullptr;
            std::size_t old_size = GetAllocatedSize(ptr);
            MemoryBase::Memcpy(new_ptr, ptr, Min<std::size_t>(old_size, new_size));
            AnsiFree(ptr);
            return new_ptr;
        }else{
            AnsiFree(ptr);
            return nullptr;
        }
    }
    if(new_size != 0) return AnsiMalloc(new_size, alignment);
    return nullptr;
}

void MallocAnsi::Free(void* ptr){
    AnsiFree(ptr);
}

std::string MallocAnsi::GetDescriptName() const{
    return "Ansi";
}

bool MallocAnsi::IsThreadSafe() const{
    return true;
}

std::size_t MallocAnsi::GetAllocatedSize(void* ptr){
    if(ptr == nullptr) return 0;
    return *(std::size_t*)((uint8_t*)ptr - sizeof(void*) - sizeof(std::size_t));
}

}
