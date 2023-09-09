#include "MallocTC.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif

namespace sablin{

MallocTC::MallocTC():
        MallocBase()
{

}

MallocTC::~MallocTC(){
}

void* MallocTC::Malloc(std::size_t size, uint32_t alignment){
#ifdef DEBUG
    std::cout << "MallocTC::Malloc" << std::endl;
#endif
    return nullptr;
}

void* MallocTC::TryMalloc(std::size_t size, uint32_t alignment){
    return nullptr;
}

void* MallocTC::Realloc(void* ptr, std::size_t new_size, uint32_t alignment){
    return nullptr;
}

void* MallocTC::TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment){
    return nullptr;
}

void MallocTC::Free(void* ptr){
#ifdef DEBUG
    std::cout << "MallocTC::Free" << std::endl;
#endif
}

std::string MallocTC::GetDescriptName() const{
    return "TC";
}

bool MallocTC::IsThreadSafe() const{
    return true;
}

std::size_t MallocTC::GetAllocatedSize(void* ptr){
    return 0;
}

}
