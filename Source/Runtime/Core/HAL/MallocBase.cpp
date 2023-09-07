#include "MallocBase.h"
#include "../Debug/Assertion.h"
#include <malloc.h>

namespace sablin{

void* UseSystemMallocForNew::operator new(std::size_t size){
    return ::malloc(size);
}

void* UseSystemMallocForNew::operator new[](std::size_t size){
    return ::malloc(size);
}

void UseSystemMallocForNew::operator delete(void* ptr){
    ::free(ptr);
}

void UseSystemMallocForNew::operator delete[](void* ptr){
    ::free(ptr);
}

void* MallocBase::TryMalloc(std::size_t size, uint32_t alignment){
    NO_IMPLEMENT_ASSERT(MallocBase, TryMalloc)
    return nullptr;
}

void* MallocBase::Realloc(void* ptr, std::size_t new_size, uint32_t alignment){
    NO_IMPLEMENT_ASSERT(MallocBase, Realloc)
    return nullptr;
}

void* MallocBase::TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment){
    NO_IMPLEMENT_ASSERT(MallocBase, TryRealloc);
    return nullptr;
}

std::string MallocBase::GetDescriptName() const{
    return "MallocBase";
}

}
