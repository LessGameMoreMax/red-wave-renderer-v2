#include "MallocAnsi.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
#include "PlatformMemory.h"
#include "../Math/MathTools.h"
namespace sablin{

MallocAnsi::MallocAnsi():
        MallocBase()
{

}

MallocAnsi::~MallocAnsi(){

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
    if(size < 16){
        alignment = Max<uint32_t>(8, alignment);
    }else{
        alignment = Max<uint32_t>(16, alignment);
    }

    void* ptr = ::malloc(sizeof(std::size_t) + sizeof(void*) + alignment + size);
    if(ptr == nullptr) return nullptr;
    void* result = Align((uint8_t*)ptr + sizeof(std::size_t) + sizeof(void*), alignment);
    *(void**)((uint8_t*)result - sizeof(void*)) = ptr;
    *(std::size_t*)((uint8_t*)result - sizeof(void*) - sizeof(size_t)) = size;
    return result;
}

void* MallocAnsi::Realloc(void* ptr, std::size_t new_size, uint32_t alignment){
    return nullptr;
}

void* MallocAnsi::TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment){
    return nullptr;
}

void MallocAnsi::Free(void* ptr){
    if(ptr == nullptr)[[unlikely]] return;
    ::free(*(void**)((uint8_t*)ptr - sizeof(void*))); 
}

std::string MallocAnsi::GetDescriptName() const{
    return "Ansi";
}

bool MallocAnsi::IsThreadSafe() const{
    return true;
}

}
