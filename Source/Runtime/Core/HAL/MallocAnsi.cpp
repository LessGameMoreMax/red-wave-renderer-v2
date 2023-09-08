#include "MallocAnsi.h"
#include "../Misc/MacroDefine.h"
#ifdef DEBUG
#include <iostream>
#endif
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
    return malloc(size);
}

void* MallocAnsi::TryMalloc(std::size_t size, uint32_t alignment){
    return nullptr;
}

void* MallocAnsi::Realloc(void* ptr, std::size_t new_size, uint32_t alignment){
    return nullptr;
}

void* MallocAnsi::TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment){
    return nullptr;
}

void MallocAnsi::Free(void* ptr){

}

std::string MallocAnsi::GetDescriptName() const{
    return "Ansi";
}

bool MallocAnsi::IsThreadSafe() const{
    return true;
}

}
