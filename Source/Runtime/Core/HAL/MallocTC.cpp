#include "MallocTC.h"

namespace sablin{

MallocTC::MallocTC():
        MallocBase()
{

}

MallocTC::~MallocTC(){
}

void* MallocTC::Malloc(std::size_t size, uint32_t alignment){
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

}

std::string MallocTC::GetDescriptName() const{
    return "TC";
}

bool MallocTC::IsThreadSafe() const{
    return true;
}

}
