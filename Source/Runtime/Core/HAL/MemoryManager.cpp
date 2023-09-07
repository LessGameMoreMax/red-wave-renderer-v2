#include "MemoryManager.h"
#include "MemoryBase.h"
#include "../Debug/Assertion.h"
namespace sablin{

void MemoryManager::Initialize(){
    ASSERT_WITH_STRING(MemoryBase::CreateMallocBase() != nullptr,
            "Failed to Create Memory Alloctor!")
}

}

